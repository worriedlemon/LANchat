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

    class ILoadable
    {
    protected:
        ILoadable() = default;

    public:
        ILoadable(ILoadable const&) = default;

        static const QMap<QString, ILoadable* (*)()> InitializeEmpty;

        virtual ~ILoadable() = default;

        virtual QMap<QString, QString>& Serialize() = 0;
        virtual void Deserialize(QMap<QString, QString>& varValueMap) = 0;
    };

    enum PacketDataType
    {
        INFO = 0x00,
        REQUEST = 0x88,
        MESSAGE = 0xFF
    };

    class AbstractTcpPacket
    {
    protected:
        AbstractTcpPacket(PacketDataType dataType);

    public:
        virtual ~AbstractTcpPacket() = default;

        byte dataType;

        virtual void Apply(MainWindow& mw, ChatClient& client) = 0;
        virtual QByteArray ToByteArray();

        static PacketDataType DeterminePacketType(AbstractTcpPacket& pkg);
        static PacketDataType DeterminePacketType(QByteArray& arr);
    };

} // namespace Connection

#endif // CONNECTION_ABSTRACTTCPPACKET_H
