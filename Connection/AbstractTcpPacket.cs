using System.Net.Sockets;

namespace LANchat.Connection
{
    public interface ILoadable { }

    public enum PacketDataType
    {
        INFO = 0x00,
        REQUEST = 0x88,
        MESSAGE = 0xFF
    }

    public abstract class AbstractTcpPacket
    {
        protected readonly byte _dataType;

        public AbstractTcpPacket(PacketDataType dataType)
        {
            _dataType = (byte)dataType;
        }

        public static PacketDataType DeterminePacketType(AbstractTcpPacket pkg) => (PacketDataType)pkg._dataType;

        public static PacketDataType DeterminePacketType(byte[] packetAsByteArray) => (PacketDataType)packetAsByteArray[0];

        public abstract void Apply(MainWindow mw, ChatClient client);

        public virtual byte[] ToByteArray() => new byte[1] { _dataType };
    }
}
