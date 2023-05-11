#ifndef MESSAGETIMEPAIR_H
#define MESSAGETIMEPAIR_H
#include <QString>

namespace Connection
{
    struct MessageTimePair
    {
        QString Message;
        long Time;

        MessageTimePair(QString message, long time)
        {
            Message = message;
            Time = time;
        }

        MessageTimePair() = default;
    };
}
#endif // MESSAGETIMEPAIR_H
