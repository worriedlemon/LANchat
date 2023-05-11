#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QException>

namespace Exceptions
{
    class CommonException : QException
    {
    public:
        QString Message;

        CommonException(QString message = "");

        void raise() const override;
        CommonException* clone() const override;
    };
}
#endif // EXCEPTION_H
