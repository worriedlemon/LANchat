#ifndef CONNECTION_REQUESTPACKET_H
#define CONNECTION_REQUESTPACKET_H

#include "abstracttypes.h"

namespace Connection
{
    class RequestPacket : public AbstractTcpPacket
    {
    public:
        RequestPacket();
        ~RequestPacket() = default;

        void Apply(MainWindow& mw, ChatClient& client) override;
    };

} // namespace Connection

#endif // CONNECTION_REQUESTPACKET_H
