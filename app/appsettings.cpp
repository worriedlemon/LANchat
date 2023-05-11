#include "appsettings.h"
#include "src/globaldefs.h"
#include "exceptions.h"
using namespace Exceptions;

#include <QDir>

AppSettings::AppSettings() { }

AppSettings AppSettings::DefaultSettings()
{
    AppSettings a;
    a.Username = QDir::home().dirName();
    a.Port = GlobalDefs::DefaultPort;
    return a;
}

bool AppSettings::IsDefaultPort() const
{
    return Port == GlobalDefs::DefaultPort;
}

QMap<QString, QString>& AppSettings::Serialize()
{
    QMap<QString, QString>* varValueMap = new QMap<QString, QString> { { "ClassName", "AppSettings" } };
    varValueMap->insert("Username", Username);
    varValueMap->insert("Port", QString::number(Port));
    return *varValueMap;
}

void AppSettings::Deserialize(QMap<QString, QString>& varValueMap)
{
    if (varValueMap["ClassName"] != "AppSettings")
    {
        throw CommonException("Field 'ClassName' is not equals to 'AppSettings'");
    }
    Username = varValueMap["Username"];
    Port = varValueMap["Port"].toUInt();
}

QString AppSettings::ToString()
{
    return QString::asprintf("'%s' on port %u", Username.toStdString().c_str(), Port);
}
