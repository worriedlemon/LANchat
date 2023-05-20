using System.Net;
using System.Net.Sockets;

namespace LANchat.Connection
{
    public class ChatClient
    {
        private string _lastName;

        public enum ChatType
        {
            INCOMING = 1,
            OUTCOMING = 2
        }


        public Socket Client;
        public string Name;
        public ChatType Type;

        public ChatClient(in TcpClient client, ChatType type = ChatType.OUTCOMING)
        {
            Client = client.Client;
            Name = string.Empty;
            Type = type;
            _lastName = GetFullName();
        }

        public void SendPacket(AbstractTcpPacket pkg) => Client.Send(pkg.ToByteArray());

        public void Close() => Client.Close();

        public void UpdateInformation(string name)
        {
            Name = name;
            _lastName = GetFullName();
        }
        
        private string GetFullName()
        {
            string tempName = Name == string.Empty ? "" : $"({Name}) ";
            string portString = (Type == ChatType.OUTCOMING ? Client.RemoteEndPoint : Client.LocalEndPoint).ToString().Split(':')[1];
            _lastName = $"{tempName}{Client.RemoteEndPoint.ToString().Split(':')[0]}:{portString}";
            return _lastName;
        }

        public override string ToString()
        {
            if (!Client.Connected) return _lastName;
            else return GetFullName();
        }
    }
}
