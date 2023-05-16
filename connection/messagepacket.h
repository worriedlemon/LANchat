#ifndef CONNECTION_MESSAGEPACKET_H
#define CONNECTION_MESSAGEPACKET_H
#include "abstracttypes.h"
#include "messagetimepair.h"

namespace Connection
{
    /// TCP packet, which contains message string and time code
    class MessagePacket : public AbstractTcpPacket
    {
        MessageTimePair MessageAndTime;
    public:
        MessagePacket(QString message);
        MessagePacket(QByteArray& bytes);
        ~MessagePacket() = default;

        void Apply(MainWindow& mw, ChatClient& client) override;
        QByteArray ToByteArray() override;

    };
} // namespace Connection

#endif // CONNECTION_MESSAGEPACKET_H
