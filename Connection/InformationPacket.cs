using System.Linq;
using System.Text.Json;
using System.Text;

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
            Information = JsonSerializer.Deserialize<AppSettings>(bytes.Skip(1).ToArray(),
                new JsonSerializerOptions() { IncludeFields = true });
        }

        public override byte[] ToByteArray()
        {
            return Encoding.UTF8.GetBytes(JsonSerializer.Serialize(Information, new JsonSerializerOptions()
            { IncludeFields = true })).Prepend(base.ToByteArray()[0]).ToArray();
        }

        public override void Apply(MainWindow mw, ChatClient client)
        {
            mw.UpdateClientInfo(client, Information);
        }
    }
}
