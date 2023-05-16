#ifndef CONNECTION_INFORMATIONPACKET_H
#define CONNECTION_INFORMATIONPACKET_H
#include "abstracttypes.h"
#include "app/appsettings.h"

namespace Connection
{
    /// TCP packet, which contains common information about host
    class InformationPacket : public AbstractTcpPacket
    {
    public:
        AppSettings Information;

        InformationPacket(AppSettings info);
        InformationPacket(QByteArray& bytes);
        ~InformationPacket() = default;

        void Apply(MainWindow& mw, ChatClient& client) override;
        QByteArray ToByteArray() override;
    };
} // namespace Connection

#endif // CONNECTION_INFORMATIONPACKET_H
