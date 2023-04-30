using System;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.Serialization.Formatters.Binary;

namespace LANchat.Connection
{
    internal class InformationPacket : AbstractTcpPacket
    {
        public AppSettings Information;

        public InformationPacket(AppSettings info) : base(PacketDataType.INFO)
        {
            Information = info;
        }

        public InformationPacket(byte[] bytes) : base(PacketDataType.INFO)
        {
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream(bytes.Skip(1).ToArray());
            Information = bf.Deserialize(ms) as AppSettings;
        }

        public override byte[] ToByteArray()
        {
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream();
            bf.Serialize(ms, Information);
            return base.ToByteArray().Concat(ms.ToArray()).ToArray();
        }

        public override void Apply(MainWindow mw, ChatClient client)
        {
            mw.UpdateClientInfo(client, Information);
        }
    }
}
