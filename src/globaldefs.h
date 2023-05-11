#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H
#include <QString>
#include <QDebug>

#ifndef qLOG
#define qLOG if (GlobalDefs::IsVerbose()) qDebug().noquote().nospace()
#endif

#ifndef asyncf
#define asyncf // Just for user convenience
#endif

typedef unsigned char byte;

class GlobalDefs
{
private:
    GlobalDefs();
    GlobalDefs(GlobalDefs const &);
    GlobalDefs operator=(GlobalDefs&);
    ~GlobalDefs();

    static GlobalDefs* instance;

    QString *applicationTitle, *applicationVersion;
    bool consoleMode, verboseMode;


public:
    static const quint16 DefaultPort = 51492;
    static const quint16 PortMaxValue = (quint16)(~0);

    static void Instantiate();

    static void SetApplicationTitle(QString title);
    static QString& GetApplicationTitle();

    static void SetApplicationVersion(QString version);
    static QString& GetApplicationVersion();

    static void SetConsoleMode(bool opt);
    static void SetVerboseMode(bool opt);

    static bool IsVerbose();
    static bool IsConsole();
};

#endif // GLOBALDEFS_H
