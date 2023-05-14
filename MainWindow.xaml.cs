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

        private readonly List<ChatClient> _connections = new List<ChatClient>();
        private readonly Dictionary<ChatClient, List<string>> _history = new Dictionary<ChatClient, List<string>>();

        public MainWindow()
        {
            InitializeComponent();
            Title = GlobalDefs.ApplicationTitle;

            _listener.Start();
            AcceptConnectionsAsync();
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
            InformationPacket info = new InformationPacket(App.Settings);
            foreach (ChatClient client in _connections)
            {
                client.SendPacket(info);
            }
        }

        private void AddConnectionButton_Click(object sender, RoutedEventArgs e) => new ConnectionDialog(this).Show();

        private void SettingsButton_Click(object sender, RoutedEventArgs e) => new SettingsDialog(this).Show();

        private void MessageTextBox_GotFocus(object sender, RoutedEventArgs e) => PlaceholderText.Content = string.Empty;

        private void MessageTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (MessageTextBox.Text == string.Empty) PlaceholderText.Content = "Write your message...";
        }

        public void ProcessMessage(string message, long time, ChatClient client = null)
        {
            string username = client == null ? App.Settings.Username : client.Name; 
            string userTimeString = $"[{username}] at {DateTime.FromBinary(time):hh:mm:ss dd/MM/yyyy}";
            if (client == null) client = _currentChat;
            _history[client].Add(userTimeString);
            _history[client].Add(message);
            if (client == _currentChat)
            {
                Messages.Items.Add(userTimeString);
                Messages.Items.Add(message);
            }
        }

        private void SendMessage(object sender, RoutedEventArgs e)
        {
            MessagePacket mp = new MessagePacket(MessageTextBox.Text);
            _currentChat.SendPacket(mp);
            ProcessMessage(MessageTextBox.Text, DateTime.Now.ToBinary());
            MessageTextBox.Text = string.Empty;
            PlaceholderText.Content = "Write your message...";
        }

        private void ChatList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            bool elementsEnabled = e.AddedItems.Count != 0;
            DisconnectButton.IsEnabled = elementsEnabled;
            MessageTextBox.IsEnabled = elementsEnabled;
            Messages.Items.Clear();
            _currentChat = elementsEnabled ? e.AddedItems[0] as ChatClient : null;
            if (_currentChat != null)
            {
                foreach (string str in _history[_currentChat])
                {
                    Messages.Items.Add(str);
                }
            }
        }

        private void DisconnectButton_Click(object sender, RoutedEventArgs e)
        {
            _currentChat.Close();
            _currentChat = null;
            Messages.Items.Clear();
            DisconnectButton.IsEnabled = false;
            MessageTextBox.IsEnabled = false;
        }

        private async void AcceptConnectionsAsync()
        {
            while (_listener != null)
            {
                TcpClient client = await _listener.AcceptTcpClientAsync();
                ChatClient connection = new ChatClient(client);
                _connections.Add(connection);
                _history.Add(connection, new List<string>());
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

                if (!await Task.Run(() => {
                    string clientName = connection.ToString();
                    try
                    {
                        count = connection.Client.Receive(buffer);
                    }
                    catch (SocketException)
                    {
                        Console.WriteLine("Connection to {0} dropped", clientName);
                        return false;
                    }
                    return count != 0;
                })) break;

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
            UpdateChatList();
        }

        public void UpdateClientInfo(ChatClient client, AppSettings info)
        {
            ChatClient found;
            if (_connections.Contains(client))
            {
                found = _connections.Find((other) => client == other);
            }
            else
            {
                _connections.Add(client);
                _history.Add(client, new List<string>());
                found = client;
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
            foreach (ChatClient connection in _connections)
            {
                connection.Close();
            }
            _connections.Clear();
        }
    }
}
