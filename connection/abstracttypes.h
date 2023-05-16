#ifndef CONNECTION_ABSTRACTTCPPACKET_H
#define CONNECTION_ABSTRACTTCPPACKET_H
#include "src/mainwindow.h"

#include <QByteArray>
#include <QMap>

#define DEFINE_NEW(Class) { #Class, []() -> ILoadable* { return new Class(); } }
#define INITIALIZE_EMPTY(Class) Connection::ILoadable::InitializeEmpty[Class]()

namespace Connection
{
    typedef unsigned char byte;
    class ChatClient;

    /// Abstract class, which should behave as serializable loadable object
    class ILoadable
    {
    protected:
        ILoadable() = default;

    public:
        ILoadable(ILoadable const&) = default;

        /// Variable that stores invokation of an empty class by its name
        static const QMap<QString, ILoadable* (*)()> InitializeEmpty;

        virtual ~ILoadable() = default;

        /// Returns map of values and their names
        virtual QMap<QString, QString>& Serialize() = 0;

        /// Fills the fields with giveb values by their names
        virtual void Deserialize(QMap<QString, QString>& varValueMap) = 0;
    };

    /// Contains possible data types of a packet
    enum PacketDataType
    {
        INFO = 0x00,
        REQUEST = 0x88,
        MESSAGE = 0xFF
    };

    /// Abstract class for a TCP packet
    class AbstractTcpPacket
    {
    protected:
        AbstractTcpPacket(PacketDataType dataType);

    public:
        virtual ~AbstractTcpPacket() = default;

        byte dataType;

        /// Applies packet from specific ChatClient to program's MainWindow
        virtual void Apply(MainWindow& mw, ChatClient& client) = 0;

        /// Returns binary representation of a packet
        virtual QByteArray ToByteArray();

        /// Determines packet type by its binary representation
        static PacketDataType DeterminePacketType(QByteArray& arr);
    };

} // namespace Connection

#endif // CONNECTION_ABSTRACTTCPPACKET_H
