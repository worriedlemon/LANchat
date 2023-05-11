#include "client.h"
#include "connection/informationpacket.h"
#include "connection/requestpacket.h"
#include "connection/messagepacket.h"

#include <QtNetwork/QHostAddress>

namespace Connection
{
    ChatClient::ChatClient(QTcpSocket* instance, MainWindow& mw)
        : Name("_ANONYMOUS_"),
          mw(mw),
          connection(instance)
    {
        connect(connection, SIGNAL(readyRead()), this, SLOT(ReceivePacket()));
    }

    void ChatClient::SendPacket(AbstractTcpPacket& pkg)
    {
        connection->write(pkg.ToByteArray());
    }

    void ChatClient::UpdateInformation(QString name)
    {
        Name = name;
    }

    const QString ChatClient::ToString() const
    {
        QString name = "";
        if (Name != "")
        {
            name = "(" + Name + ") ";
        }
        return name + QHostAddress(connection->localAddress().toIPv4Address()).toString()
                + ":" + QString::number(connection->localPort());
    }

    void ChatClient::ReceivePacket()
    {
        QByteArray bytes = connection->readAll();
        PacketDataType dataType = AbstractTcpPacket::DeterminePacketType(bytes);
        bytes = bytes.remove(0, 1);
        qLOG << "Received packet of type "
             << (dataType == 0xff ? "MESSAGE" : dataType == 0x88 ? "REQUEST" : "INFO")
             << ": " << QString(bytes);

        AbstractTcpPacket* pkg;
        switch (dataType)
        {
        case PacketDataType::REQUEST:
            pkg = new RequestPacket();
            break;
        case PacketDataType::INFO:
            pkg = new InformationPacket(bytes);
            break;
        case PacketDataType::MESSAGE:
            pkg = new MessagePacket(bytes);
            break;
        default:
            qLOG << "Packet type unknown - ignored";
            return;
        }
        pkg->Apply(mw, *this);
        delete pkg;
    }
} // namespace Connection
