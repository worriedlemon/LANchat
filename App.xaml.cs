using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Windows;

namespace LANchat
{
    /// <summary>
    /// App internal model code
    /// </summary>
    public partial class App : Application
    {
        public const string SavedSettingsDirectory = @".\saved\";
        public const string SettingsFilePath = SavedSettingsDirectory + "settings.xml";

        public static AppSettings Settings;

        private MainWindow _mw;

        private void ApplicationStartup(object sender, StartupEventArgs e)
        {
            if (!ArgumentProcessor.ProcessArguments(e.Args))
            {
                try
                {
                    Settings = FileManager.LoadFromXML<AppSettings>(SettingsFilePath);
                }
                catch (Exception exc) when (exc is FileNotFoundException || exc is DirectoryNotFoundException)
                {
                    Settings = AppSettings.DefaultSettings();
                    if (exc is DirectoryNotFoundException) Directory.CreateDirectory(SavedSettingsDirectory);
                    FileManager.SaveToXML(Settings, SettingsFilePath);
                }

                _mw = new MainWindow();
                _mw.Show();
            }
            else Shutdown();
        }
    }
}
