using LANchat.Connection;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace LANchat
{
    /// <summary>
    /// Логика взаимодействия для ConnectionDialogWindow.xaml
    /// </summary>
    public partial class ConnectionDialog : Window
    {
        private readonly MainWindow _pw;

        public ConnectionDialog(MainWindow parent)
        {
            _pw = parent;
            InitializeComponent();
            PortSegmentBlock(true);
        }

        private void PortSegmentBlock(bool block)
        {
            if (block) PortSegment.Text = GlobalDefs.DefaultPort.ToString();
            PortSegment.IsEnabled = !block;
        }

        private void PortSegment_TextInput(object sender, TextCompositionEventArgs e)
        {
            if (Convert.ToUInt32(PortSegment.Text) > ushort.MaxValue)
            {
                PortSegment.Text = ushort.MaxValue.ToString();
            }
        }

        private void CheckIPSegment(object sender, TextChangedEventArgs e)
        {
            TextBox ipSegment = sender as TextBox;
            try
            {
                Convert.ToByte(ipSegment.Text);
            }
            catch (OverflowException)
            {
                ipSegment.Text = byte.MaxValue.ToString();
            }
            catch (FormatException)
            {
                ipSegment.Text = string.Empty;
            }
        }

        private void AddConnectionButtonCLick(object sender, RoutedEventArgs e)
        {
            try
            {
                IPAddress ipAddress = IPAddress.Parse($"{IPByte1.Text}.{IPByte2.Text}.{IPByte3.Text}.{IPByte4.Text}");
                ChatClient newConnection = new ChatClient(new TcpClient(ipAddress.ToString(), Convert.ToUInt16(PortSegment.Text)));
                newConnection.SendPacket(new RequestPacket());
                 _pw.StartReceivingFromConnection(newConnection);
                Close();
            }
            catch (FormatException)
            {
                new ExceptionDialog("Entered IP address has wrong format!").ShowDialog();
            }
            catch (Exception exc) when (exc is SocketException || exc is ArgumentException)
            {
                new ExceptionDialog(exc.Message).ShowDialog();
            }
        }

        private void CancelButtonClick(object sender, RoutedEventArgs e) => Close();

        private void DefaultPortCheckboxChecked(object sender, RoutedEventArgs e)
        {
            PortSegmentBlock(DefaultPortCheckbox.IsChecked.Value);
        }
    }
}
