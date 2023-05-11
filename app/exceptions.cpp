#include "exceptions.h"

namespace Exceptions
{
    CommonException::CommonException(QString message) : QException()
    {
        Message = message;
    }

    void CommonException::raise() const
    {
        throw *this;
    }

    CommonException* CommonException::clone() const
    {
        return new CommonException(Message);
    }
}
