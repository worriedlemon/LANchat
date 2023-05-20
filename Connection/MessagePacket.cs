using System;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace LANchat.Connection
{
    public class MessagePacket : AbstractTcpPacket
    {

        [Serializable]
        public class MessageTimePair
        {
            public string Message;
            public long Time;

            [JsonIgnore]
            public static DateTime Since = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);

            public MessageTimePair(string message, long time)
            {
                Message = message;
                Time = time;
            }
        }

        public MessageTimePair MessageAndTime;

        public MessagePacket(string message) : base(PacketDataType.MESSAGE)
        {
            long secondsFromEpoch = (long)(DateTime.Now.ToUniversalTime() - MessageTimePair.Since).TotalSeconds;
            MessageAndTime = new MessageTimePair(message, secondsFromEpoch);
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
            mw.ProcessMessage(MessageAndTime.Message, MessageAndTime.Time, client);
        }
    }
}
