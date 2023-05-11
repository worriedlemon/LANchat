#include "messagepacket.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include "client.h"

namespace Connection
{
    MessagePacket::MessagePacket(QString message)
        : AbstractTcpPacket(PacketDataType::MESSAGE),
          MessageAndTime(message, QDateTime::currentSecsSinceEpoch())
    { }

    MessagePacket::MessagePacket(QByteArray& bytes)
        : AbstractTcpPacket(PacketDataType::MESSAGE)
    {
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        MessageAndTime.Message = doc.object().value("Message").toString();
        MessageAndTime.Time = doc.object().value("Time").toInt();
    }

    void MessagePacket::Apply(MainWindow& mw, ChatClient& client)
    {
        mw.ProcessMessage(MessageAndTime, &client);
    }

    QByteArray MessagePacket::ToByteArray()
    {
        QJsonDocument doc(QJsonObject{
            { "Message", MessageAndTime.Message },
            { "Time", QJsonValue((qint64)MessageAndTime.Time) }
        });
        return AbstractTcpPacket::ToByteArray().append(doc.toJson());
    }
} // namespace Connection
