#include "client.h"
#include "connection/informationpacket.h"
#include "connection/requestpacket.h"
#include "connection/messagepacket.h"

#include <QtNetwork/QHostAddress>

namespace Connection
{
    ChatClient::ChatClient(QTcpSocket* instance, MainWindow& mw, ChatType t)
        : Name(""),
          mw(mw),
          connection(instance)
    {
        Type = t;
        connect(connection, SIGNAL(readyRead()), this, SLOT(ReceivePacket()));
        connect(connection, SIGNAL(disconnected()), this, SLOT(CloseConnection()));
    }

    ChatClient::~ChatClient()
    {
        disconnect(connection, SIGNAL(readyRead()), this, SLOT(ReceivePacket()));
        if (connection->state() == QAbstractSocket::ConnectedState) connection->close();
        delete connection;
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
        QString portString = QString::number(Type == ChatType::INCOMING ? connection->localPort() : connection->peerPort());
        return name + QHostAddress(connection->peerAddress().toIPv4Address()).toString() + ":" + portString;
    }

    void ChatClient::ReceivePacket()
    {
        QByteArray bytes = connection->readAll();

        packetleft:
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

        if (dataType == PacketDataType::REQUEST && bytes.length() > 0) goto packetleft;
    }

    void ChatClient::CloseConnection()
    {
        mw.RemoveConnection(this);
    }
} // namespace Connection
