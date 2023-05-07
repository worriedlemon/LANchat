using System.Linq;
using System.Text;
using System.Text.Json;

namespace LANchat.Connection
{
    public class MessagePacket : AbstractTcpPacket
    {
        [System.Serializable]
        public class MessageTimePair
        {
            public string Message;
            public long Time;

            public MessageTimePair(string message, long time)
            {
                Message = message;
                Time = time;
            }
        }

        public MessageTimePair MessageAndTime;

        public MessagePacket(string message) : base(PacketDataType.MESSAGE)
        {
            MessageAndTime = new MessageTimePair(message, System.DateTime.Now.ToBinary());
        }

        public MessagePacket(byte[] bytes) : base(PacketDataType.MESSAGE)
        {
            MessageAndTime = JsonSerializer.Deserialize<MessageTimePair>(bytes.Skip(1).ToArray(),
                new JsonSerializerOptions() { IncludeFields = true });
        }

        public override byte[] ToByteArray()
        {
            return Encoding.UTF8.GetBytes(JsonSerializer.Serialize(MessageAndTime, new JsonSerializerOptions()
            { IncludeFields = true })).Prepend(base.ToByteArray()[0]).ToArray();
        }

        public override void Apply(MainWindow mw, ChatClient client)
        {
            mw.ChatWindow.Text += $"[{client.Name}]: {MessageAndTime.Message}\n";
            //mw.TimeWindow.Text += $"{MessageAndTime.Time}\n";
        }
    }
}
