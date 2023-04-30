using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;

namespace LANchat.Connection
{
    public class MessagePacket : AbstractTcpPacket
    {
        [Serializable]
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
            MessageAndTime = new MessageTimePair(message, DateTime.Now.ToBinary());
        }

        public MessagePacket(byte[] bytes) : base(PacketDataType.MESSAGE)
        {
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream(bytes.Skip(1).ToArray());
            MessageAndTime = bf.Deserialize(ms) as MessageTimePair;
        }

        public override byte[] ToByteArray()
        {
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream();
            bf.Serialize(ms, MessageAndTime);
            return base.ToByteArray().Concat(ms.ToArray()).ToArray();
        }

        public override void Apply(MainWindow mw, ChatClient client)
        {
            mw.ChatWindow.Text += $"[{client.Name}]: {MessageAndTime.Message}\n";
            //mw.TimeWindow.Text += $"{MessageAndTime.Time}\n";
        }
    }
}
