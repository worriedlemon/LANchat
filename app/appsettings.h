#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include "connection/abstracttypes.h"

/// Class for storing Username and Port of a host
class AppSettings : public Connection::ILoadable
{
public:
    QString Username;
    quint16 Port;

public:
    AppSettings();

    /// Returns default value, which are home directory name for username, and port = 51492
    static AppSettings DefaultSettings();

    /// Compares port to 51492
    bool IsDefaultPort() const;

    /// Returns information as a string
    QString ToString();

    QMap<QString, QString>& Serialize() override;
    void Deserialize(QMap<QString, QString>& varValueMap) override;
};

#endif // APPSETTINGS_H
