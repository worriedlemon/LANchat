#include "informationpacket.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Connection
{
    InformationPacket::InformationPacket(AppSettings info)
        : AbstractTcpPacket(PacketDataType::INFO),
          Information(info)
    { }

    InformationPacket::InformationPacket(QByteArray& bytes) : AbstractTcpPacket(PacketDataType::INFO)
    {
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        Information.Username = doc.object().value("Username").toString();
        Information.Port = doc.object().value("Port").toInt();
    }

    void InformationPacket::Apply(MainWindow &mw, ChatClient &client)
    {
        mw.UpdateClientInfo(client, Information);
    }

    QByteArray InformationPacket::ToByteArray()
    {
        QJsonDocument doc(QJsonObject{
            { "Username", Information.Username },
            { "Port", Information.Port }
        });
        return AbstractTcpPacket::ToByteArray().append(doc.toJson());
    }
} // namespace Connection
