#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui
{
    class ConnectionDialog;
}

namespace Connection
{
    class ChatClientItem;
}

/// Simple dialog for connecting to a host
class ConnectionDialog : public QDialog
{
    Q_OBJECT

    const QIntValidator ipSegmentValidator, portValidator;
    Ui::ConnectionDialog *ui;

public:
    explicit ConnectionDialog(MainWindow* parent = nullptr);
    ~ConnectionDialog();

private:
    /// Checks whether entered values describe IPv4 Address
    bool CheckIPBytes();

signals:
    /// Signal, which emits when host it successfully connected
    void ConnectionReady(Connection::ChatClientItem* item);

private slots:
    /// Enables and disables port, depends on 'Default Port' set value
    void EnablePort(bool checked);

    /// Slot for validating port field
    void CheckPort();

public slots:
    /// Parses given information about the host and tries to connect to it
    void accept() override;
};

#endif // CONNECTIONDIALOG_H
