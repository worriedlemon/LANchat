using LANchat.Connection;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System;
using System.Linq;

namespace LANchat
{
    public partial class MainWindow : Window
    {
        public struct View
        {
            public List<ChatClient> Items;
        }

        private TcpListener _listener = new TcpListener(IPAddress.Any, App.Settings.Port);
        private ChatClient _currentChat = null;

        private List<ChatClient> _connections = new List<ChatClient>();

        public MainWindow()
        {
            InitializeComponent();
            _listener.Start();
            AcceptConnectionsAsync();
            Title = GlobalDefs.ApplicationTitle;
            UpdateAppSettingsInfo();
        }

        public void RestartPortListener()
        {
            _listener.Stop();
            _listener = new TcpListener(IPAddress.Any, App.Settings.Port);
            _listener.Start();
            AcceptConnectionsAsync();
        }

        public void UpdateAppSettingsInfo()
        {
            AppSettingInfo.Content = App.Settings.ToString();
        }

        private void AddConnectionButton_Click(object sender, RoutedEventArgs e) => new ConnectionDialog(this).Show();

        private void SettingsButton_Click(object sender, RoutedEventArgs e) => new SettingsDialog(this).Show();


        private void MessageTextBox_GotFocus(object sender, RoutedEventArgs e) => PlaceholderText.Content = string.Empty;

        private void MessageTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (MessageTextBox.Text == string.Empty) PlaceholderText.Content = "Write your message...";
        }

        private void SendMessage(object sender, RoutedEventArgs e)
        {
            MessagePacket mp = new MessagePacket(MessageTextBox.Text);
            _currentChat.SendPacket(mp);
            ChatWindow.Text += $"[{App.Settings.Username}]: {MessageTextBox.Text}\n";
            MessageTextBox.Text = string.Empty;
            PlaceholderText.Content = "Write your message...";
        }

        private void ChatList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            _currentChat = e.AddedItems[0] as ChatClient;
            MessageWindowLayout.Visibility = Visibility.Visible;
            foreach (ChatClient removedChat in e.RemovedItems) removedChat.Close();
        }

        private async void AcceptConnectionsAsync()
        {
            while (_listener != null)
            {
                TcpClient client = await _listener.AcceptTcpClientAsync();
                ChatClient connection = new ChatClient(client);
                _connections.Add(connection);
                UpdateChatList();
                StartReceivingFromConnection(connection);
            }
        }

        public async void StartReceivingFromConnection(ChatClient connection)
        {
            while (connection.Client.Connected)
            {
                byte[] buffer = new byte[1024];
                int count = 0;

                await Task.Run(() => {
                    try
                    {
                        count = connection.Client.Receive(buffer);
                    }
                    catch (SocketException)
                    {
                        Console.WriteLine("Connection to {0} dropped", connection);
                    }
                });

                AbstractTcpPacket packet;
                switch ((PacketDataType)buffer[0])
                {
                    case PacketDataType.REQUEST:
                        packet = new RequestPacket();
                        break;
                    case PacketDataType.INFO:
                        packet = new InformationPacket(buffer.Take(count).ToArray());
                        break;
                    case PacketDataType.MESSAGE:
                        packet = new MessagePacket(buffer.Take(count).ToArray());
                        break;
                    default:
                        Console.WriteLine("Unknown packet caught - data type is wrong. Skipping...");
                        continue;
                }
                Console.WriteLine("Detected packet {0}", (PacketDataType)buffer[0]);
                packet.Apply(this, connection);
            }
            _connections.Remove(connection);
        }

        public void UpdateClientInfo(ChatClient client, AppSettings info)
        {
            ChatClient found;
            try
            {
                found = _connections.Find((other) =>
                {
                    return other.Client.LocalEndPoint.Equals(client.Client.RemoteEndPoint);
                });
            }
            catch (ArgumentNullException)
            {
                found = client;
                _connections.Add(found);
            }
            found.UpdateInformation(info.Username);
            UpdateChatList();
        }

        private void UpdateChatList()
        {
            ChatList.ItemsSource = null;
            ChatList.ItemsSource = _connections;
        }

        private void MessageTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            SendMessageButton.IsEnabled = MessageTextBox.Text != string.Empty;
        }

        private void MessageWindowLayout_Initialized(object sender, System.EventArgs e)
        {
            ChatWindow.Text = string.Empty;
            MessageWindowLayout.Visibility = Visibility.Hidden;
        }

        private void MessageTextBox_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e != null && e.Key == System.Windows.Input.Key.Return && SendMessageButton.IsEnabled)
            {
                SendMessage(sender, null);
                PlaceholderText.Content = string.Empty;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //if (new ExitDialog().ShowDialog().Value == false) return;
            foreach (ChatClient connection in _connections)
            {
                connection.Close();
            }
            _connections.Clear();
        }
    }
}
