using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;

namespace LANchat
{
    public partial class SettingsDialog : Window
    {
        private readonly MainWindow _pw;

        public SettingsDialog(MainWindow parentWindow)
        {
            _pw = parentWindow;
            InitializeComponent();
            UsernameTextBox.Text = App.Settings.Username;
            PortSegment.Text = App.Settings.Port.ToString();
            PortSegmentBlock((DefaultPortCheckbox.IsChecked = App.Settings.IsDefaultPort).Value);
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

        private void CancelButtonClick(object sender, RoutedEventArgs e) => Close();

        private void DefaultPortCheckboxChecked(object sender, RoutedEventArgs e)
        {
            PortSegmentBlock(DefaultPortCheckbox.IsChecked.Value);
        }

        private bool UsernameValidator()
        {
            return (UsernameTextBox.Text.Length >= 4 || UsernameTextBox.Text.Length <= 24);
        }

        private void SaveSettingsButtonCLick(object sender, RoutedEventArgs e)
        {
            ushort newPort = Convert.ToUInt16(PortSegment.Text);

            if (_pw.HasConnections() && App.Settings.Port != newPort)
            {
                MessageBox.Show(
                    "You cannot change port if there are connected chat clients.",
                    "Port change error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                    );
                return;
            }
            else
            {
                App.Settings.Port = newPort;
                _pw.RestartPortListener();
            }

            if (!UsernameValidator())
            {
                MessageBox.Show(
                    "Username should contain from 4 to 24 latin characters, including digits and underscore.",
                    "Username invalid",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                    );
                return;
            }
            else App.Settings.Username = UsernameTextBox.Text;

            FileManager.SaveToXML(App.Settings, App.SettingsFilePath);
            _pw.UpdateAppSettingsInfo();
            Close();
        }
    }
}
