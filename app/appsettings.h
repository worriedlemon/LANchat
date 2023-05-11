#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include "connection/abstracttypes.h"

class AppSettings : public Connection::ILoadable
{
public:
    QString Username;
    quint16 Port;

public:
    AppSettings();

    static AppSettings DefaultSettings();

    bool IsDefaultPort() const;

    QString ToString();

    QMap<QString, QString>& Serialize() override;
    void Deserialize(QMap<QString, QString>& varValueMap) override;
};

#endif // APPSETTINGS_H
