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

class ConnectionDialog : public QDialog
{
    Q_OBJECT

    const QIntValidator ipSegmentValidator, portValidator;
    Ui::ConnectionDialog *ui;

public:
    explicit ConnectionDialog(MainWindow* parent = nullptr);
    ~ConnectionDialog();

private:
    bool CheckIPBytes();

signals:
    void ConnectionReady(Connection::ChatClientItem* item);

private slots:
    void EnablePort(bool checked);
    void CheckPort();

public slots:
    void accept() override;
};

#endif // CONNECTIONDIALOG_H
