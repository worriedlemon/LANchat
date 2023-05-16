#ifndef ARGUMENTPROCESSOR_H
#define ARGUMENTPROCESSOR_H
#include<QString>

/// Static class for processing terminal arguments
class ArgumentProcessor
{
private:
    ArgumentProcessor() = default;

    /// Enum of possible terminal arguments
    enum PossibleArguments
    {
        VERSION = 'v',
        VERBOSE = 'V',
        NO_GUI = 'n',
        HELP = 'h',
        OTHER = '\0'
    };

    /// Method for decomposing string-like arguments, starting with '--'
    static PossibleArguments FullArgumentsTranslation(QString arg);

    /// Method for decomposing char arguments, starting with '-'
    static PossibleArguments ShortArgumentsTranslation(QChar argChar);

public:
    /// Only public function of this class meant to decompose arguments passed in the terminal
    static bool ProcessArguments(int argumentCount, char* arguments[]);
};

#endif // ARGUMENTPROCESSOR_H
