using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace LANchat
{
    /// <summary>
    /// Dialog, which appers at screen when exception is being thrown
    /// </summary>
    public partial class ExceptionDialog : Window
    {
        public ExceptionDialog(string message, string title = "Error")
        {
            InitializeComponent();
            Title = title;
            ExceptionMessage.Text = message;
        }

        private void CloseDialog(object sender, RoutedEventArgs e) => Close();
    }
}
