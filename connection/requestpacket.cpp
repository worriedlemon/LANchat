#include "requestpacket.h"
#include "informationpacket.h"
#include "client.h"

extern AppSettings SETTINGS;

namespace Connection
{
    RequestPacket::RequestPacket() : AbstractTcpPacket(PacketDataType::REQUEST) { }

    void RequestPacket::Apply(MainWindow& mw, ChatClient& client)
    {
        InformationPacket information(SETTINGS);
        client.SendPacket(information);
    }
}// namespace Connection
