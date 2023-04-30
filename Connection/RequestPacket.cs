using System.Net.Sockets;

namespace LANchat.Connection
{
    public class RequestPacket : AbstractTcpPacket
    {
        public RequestPacket() : base(PacketDataType.REQUEST) { }

        public override void Apply(MainWindow mw, ChatClient client)
        {
            client.Client.Send(new InformationPacket(App.Settings).ToByteArray());
        }
    }
}
