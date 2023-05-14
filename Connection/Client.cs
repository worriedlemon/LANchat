using System.Net.Sockets;

namespace LANchat.Connection
{
    public class ChatClient
    {
        public Socket Client;
        public string Name;

        public ChatClient(in TcpClient client)
        {
            Client = client.Client;
            Name = string.Empty;
        }

        public void SendPacket(AbstractTcpPacket pkg) => Client.Send(pkg.ToByteArray());

        public void Close() => Client.Close();

        public void UpdateInformation(string name) => Name = name;

        public override string ToString()
        {
            string tempName = "";
            if (Name != string.Empty)
            {
                tempName = $"({Name}) ";
            }
            return $"{tempName}{Client.RemoteEndPoint}";
        }
    }
}
