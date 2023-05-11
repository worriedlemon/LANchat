#include "src/argumentprocessor.h"
#include "src/globaldefs.h"
#include <QDebug>

typedef unsigned int Flag;

ArgumentProcessor::PossibleArguments ArgumentProcessor::FullArgumentsTranslation(QString arg)
{
    if (arg.length() == 0) return OTHER;

    if (arg == "version") return VERSION;
    else if (arg == "verbose") return VERBOSE;
    else if (arg == "no_gui") return NO_GUI;
    else if (arg == "help") return HELP;
    else return OTHER;
}

ArgumentProcessor::PossibleArguments ArgumentProcessor::ShortArgumentsTranslation(QChar argChar)
{
    uchar opt = argChar.cell();

    if (opt == 'v' || opt == 'V' || opt == 'n' || opt == 'h')
        return PossibleArguments(opt);
    else return OTHER;
}

void PrintHelp()
{
    qDebug() << "Usage: LANchat [option]\n";
    qDebug() << "Options (for [option]):";
    qDebug() << "\t-v (--version)\t\tPrint LANchat version";
    qDebug() << "\t-V (--verbose)\t\tStart LANchat in verbose mode";
    qDebug() << "\t-n (--no_gui)\t\tStart LANchat in console mode (also enables verbose mode)";
    qDebug() << "\t-h (--help)\t\tPrint this message";
}

bool ArgumentProcessor::ProcessArguments(int argumentCount, char* arguments[])
{
    Flag options = 0;
    for (int i = 1; i < argumentCount; i++)
    {
        QString currentArg(arguments[i]);
        PossibleArguments option;

        if (currentArg.startsWith("--"))
        {
            option = FullArgumentsTranslation(currentArg.right(currentArg.length() - 2));
        }
        else if (currentArg.startsWith("-"))
        {
            if (currentArg.length() == 1 || currentArg.length() > 2) option = OTHER;
            else option = ShortArgumentsTranslation(currentArg[1]);
        }
        else option = OTHER;

        switch (option)
        {
        case VERSION:
            qDebug().noquote() << "LANchat ver." << GlobalDefs::GetApplicationVersion();
            options |= 1;
            break;
        case NO_GUI:
            if (GlobalDefs::IsConsole())
            {
                qDebug() << "Duplicate argument" << currentArg << "skipped";
                break;
            }
            GlobalDefs::SetConsoleMode(true);
            qDebug() << "Console mode 'ON'";
            options |= 2;
        case VERBOSE:
            if (GlobalDefs::IsVerbose()) break;
            GlobalDefs::SetVerboseMode(true);
            qDebug() << "Verbose mode 'ON'";
            options |= 4;
            break;
        case HELP:
            PrintHelp();
            options |= 1;
            break;
        case OTHER:
        default:
            qDebug() << "Unrecognized argument" << currentArg << "skipped";
        }
    }
    return options == 1;
}
