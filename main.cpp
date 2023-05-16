#include "src/globaldefs.h"
#include "src/argumentprocessor.h"
#include "src/mainwindow.h"

#include "app/noguimode.h"
#include "app/appsettings.h"
#include "app/filemanager.h"

#include <QApplication>
#include <QDir>
#include <QTranslator>

const QString SETTINGS_DIR("./saved");

AppSettings SETTINGS;
QString SETTINGS_PATH = SETTINGS_DIR + "/settings.xml";

/// Enables global variables.
/// Actually should also content settings information, but for now they are global variables.
void SetGlobalVariables()
{
    GlobalDefs::Instantiate();
    GlobalDefs::SetApplicationTitle("LANchat");
    GlobalDefs::SetApplicationVersion("1.0");
    //GlobalDefs::SetSettingsPath(QDir::homePath() + "/.config/lanchat/settings.xml");
}

/// Loading application settings, e.g. Username, Port
void InitializeApplicationSettings()
{
    qLOG << "Loading settings file: " << SETTINGS_PATH;
    try
    {
        SETTINGS = FileManager::LoadFromXML<AppSettings>(SETTINGS_PATH);
        qLOG << "Previous settings loaded.";
    }
    catch (CommonException e)
    {
        qLOG << "Settings file problem: " << e.Message << ".";
        qLOG << "Creating directory... " << (QDir().mkdir(SETTINGS_DIR) ? "OK" : "Aborted");
        SETTINGS = AppSettings::DefaultSettings();
        FileManager::SaveToXML<AppSettings>(SETTINGS, SETTINGS_PATH);
        qLOG << "Creating new file... OK";
    }

}

/// Program entry point
int main(int argc, char *argv[])
{
    int returnCode = 0;
    QCoreApplication* application = nullptr;
    MainWindow* w = nullptr;

    SetGlobalVariables();

    if (!ArgumentProcessor::ProcessArguments(argc, argv))
    {
        qLOG << "Starting " << GlobalDefs::GetApplicationTitle() << "...";
        qLOG << "Powered by Qt " << QT_VERSION_STR << ".\n";
        InitializeApplicationSettings();

        if (GlobalDefs::IsConsole()) NoGUIMode::Enter();
        else
        {
            application = new QApplication(argc, argv);
            w = new MainWindow();
            w->show();
            returnCode = application->exec();
        }
    }

    delete w;
    delete application;

    qLOG << GlobalDefs::GetApplicationTitle() << " finished its work.";

    return returnCode;
}
