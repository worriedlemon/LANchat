#ifndef ARGUMENTPROCESSOR_H
#define ARGUMENTPROCESSOR_H
#include<QString>

class ArgumentProcessor
{
private:
    ArgumentProcessor() = default;

    enum PossibleArguments
    {
        VERSION = 'v',
        VERBOSE = 'V',
        NO_GUI = 'n',
        HELP = 'h',
        OTHER = '\0'
    };

    static PossibleArguments FullArgumentsTranslation(QString arg);
    static PossibleArguments ShortArgumentsTranslation(QChar argChar);

public:
    static bool ProcessArguments(int argumentCount, char* arguments[]);
};

#endif // ARGUMENTPROCESSOR_H
