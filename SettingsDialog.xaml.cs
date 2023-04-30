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
        }
        private void PortSegment_Initialized(object sender, EventArgs e)
        {
            PortSegmentBlock(App.Settings.IsDefaultPort);
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

        private void SaveSettingsButtonCLick(object sender, RoutedEventArgs e)
        {
            App.Settings.Username = UsernameTextBox.Text;
            App.Settings.Port = Convert.ToUInt16(PortSegment.Text);
            FileManager.SaveToXML(App.Settings, App.SettingsFilePath);
            _pw.UpdateAppSettingsInfo();
            Close();
        }
    }
}
