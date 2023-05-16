#ifndef CONNECTION_REQUESTPACKET_H
#define CONNECTION_REQUESTPACKET_H

#include "abstracttypes.h"

namespace Connection
{
    /// TCP packet, which only purpose is to be sent to a host to get information packet from it
    class RequestPacket : public AbstractTcpPacket
    {
    public:
        RequestPacket();
        ~RequestPacket() = default;

        void Apply(MainWindow& mw, ChatClient& client) override;
    };

} // namespace Connection

#endif // CONNECTION_REQUESTPACKET_H
