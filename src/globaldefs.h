#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H
#include <QString>
#include <QDebug>

// This macro write all information to terminal (used when the program is in verbose mode)
#ifndef qLOG
#define qLOG if (GlobalDefs::IsVerbose()) qDebug().noquote().nospace()
#endif

/// Type defenition for a byte (8-bit) value
typedef unsigned char byte;

/// Static class, which has all global information about program (e.g. name, version, modes etc.)
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

    /// Creates a static instance
    static void Instantiate();

    /// Sets the application title
    static void SetApplicationTitle(QString title);
    /// Returns the application title
    static QString& GetApplicationTitle();

    /// Sets the application version
    static void SetApplicationVersion(QString version);
    /// Returns the application version
    static QString& GetApplicationVersion();

    /// Sets console mode (a.k.a no-GUI mode)
    static void SetConsoleMode(bool opt);
    /// Sets verbose mode (logs everything important in terminal)
    static void SetVerboseMode(bool opt);

    /// Returns true if the app is set to verbose mode
    static bool IsVerbose();
    /// Returns true if the app is set to console mode
    static bool IsConsole();
};

#endif // GLOBALDEFS_H
