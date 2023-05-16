#include "abstracttypes.h"
#include "app/appsettings.h"

namespace Connection
{
    AbstractTcpPacket::AbstractTcpPacket(PacketDataType dataType)
    {
        this->dataType = (byte)dataType;
    }

    QByteArray AbstractTcpPacket::ToByteArray()
    {
        return *(new QByteArray(1, dataType));
    }

    PacketDataType AbstractTcpPacket::DeterminePacketType(QByteArray &arr)
    {
        return PacketDataType(byte(arr[0]));
    }

    const QMap<QString, ILoadable* (*)()> ILoadable::InitializeEmpty = {
        DEFINE_NEW(AppSettings)
    };
} // namespace Сonnection
