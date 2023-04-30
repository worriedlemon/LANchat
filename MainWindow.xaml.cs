using LANchat.Connection;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System;
using System.Threading;

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

        private CancellationToken _asyncCancelToken = CancellationToken.None; 

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

                await Task.Run(() => { connection.Client.Receive(buffer); });

                AbstractTcpPacket packet;
                switch ((PacketDataType)buffer[0])
                {
                    case PacketDataType.REQUEST:
                        packet = new RequestPacket();
                        break;
                    case PacketDataType.INFO:
                        packet = new InformationPacket(buffer);
                        break;
                    case PacketDataType.MESSAGE:
                        packet = new MessagePacket(buffer);
                        break;
                    default:
                        Console.WriteLine("Unknown packet caught - data type is wrong. Skipping...");
                        continue;
                }
                Console.WriteLine("Detected packet {0}", (PacketDataType)buffer[0]);
                packet.Apply(this, connection);

                //if ((PacketDataType)buffer[0] == PacketDataType.REQUEST) connection.Close();
            }
            _connections.Remove(connection);
        }

        public void UpdateClientInfo(ChatClient client, AppSettings info)
        {
            ChatClient found = _connections.Find((such) =>
            {
                string firstIp = such.Client.LocalEndPoint.ToString(), secondIp = client.Client.LocalEndPoint.ToString();
                return firstIp.Substring(0, firstIp.Length - 6) == secondIp.Substring(0, secondIp.Length - 6);
            });
            found.UpdateInformation(info.Username);
            UpdateChatList();
        }

        private void UpdateChatList()
        {
            //ChatList.ItemsSource = null;
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
    }
}
