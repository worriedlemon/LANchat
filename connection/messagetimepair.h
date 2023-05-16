#ifndef MESSAGETIMEPAIR_H
#define MESSAGETIMEPAIR_H
#include <QString>

namespace Connection
{
    /// Simple data struct of a message and time
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
