#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "globaldefs.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class AppSettings;

namespace Connection
{
    class ChatClient;
    class ChatClientItem;
    struct MessageTimePair;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QTcpServer listener;
    Connection::ChatClientItem* currentChat;
    QMap<Connection::ChatClient*, QList<QString>> history;

public:
    MainWindow();
    ~MainWindow();

    QListWidget& GetChatList() const;
    void UpdateCurrentSessionInfo();
    void ProcessMessage(Connection::MessageTimePair const & pair, Connection::ChatClient* client = nullptr);
    void UpdateClientInfo(Connection::ChatClient& client, AppSettings& information);

private:
    void ProcessMessage(QString& timeUserString, QString& messageString);

private slots:
    void EnableSendButton();
    void ShowSettingsDialog();
    void ShowConnectionDialog();
    void AcceptConnection();
    void ShowChatFor(QListWidgetItem* item);
    void SendMessage();
    void RemoveConnection();

public slots:
    void AddConnection(Connection::ChatClientItem* item);
};
#endif // MAINWINDOW_H
