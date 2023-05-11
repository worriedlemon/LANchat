#include "src/globaldefs.h"

GlobalDefs* GlobalDefs::instance = nullptr;

GlobalDefs::GlobalDefs()
    : applicationTitle(new QString("MyApplication")),
      applicationVersion(new QString("0.0a")),
      consoleMode(false),
      verboseMode(false)
{ }

GlobalDefs::~GlobalDefs()
{
    delete applicationTitle;
    delete applicationVersion;
}

/// Creates single instance of a GlobalDefs class
void GlobalDefs::Instantiate()
{
    if (instance == nullptr)
    {
        instance = new GlobalDefs();
    }
}

/// Sets application title
void GlobalDefs::SetApplicationTitle(QString title)
{
    instance->applicationTitle = new QString(title);
}

/// Sets application version
void GlobalDefs::SetApplicationVersion(QString version)
{
    instance->applicationVersion = new QString(version);
}

/// Gets application title
QString& GlobalDefs::GetApplicationTitle()
{
    return *instance->applicationTitle;
}

/// Gets application version
QString& GlobalDefs::GetApplicationVersion()
{
    return *instance->applicationVersion;
}

/// Sets console mode
void GlobalDefs::SetConsoleMode(bool opt)
{
    instance->consoleMode = true;
}

/// Sets verbose mode
void GlobalDefs::SetVerboseMode(bool opt)
{
    instance->verboseMode = opt;
}

/// Checks whether application is set to console mode
bool GlobalDefs::IsConsole()
{
    return instance->consoleMode;
}

/// Checks whether application is set to verbose mode
bool GlobalDefs::IsVerbose()
{
    return instance->verboseMode;
}
