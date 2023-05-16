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

/// Main window - central class of the program
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

    /// Updates text of the current session
    void UpdateCurrentSessionInfo();
    /// Process message time pair from a client
    void ProcessMessage(Connection::MessageTimePair const & pair, Connection::ChatClient* client = nullptr);
    /// Updates client name
    void UpdateClientInfo(Connection::ChatClient& client, AppSettings& information);
    /// Writes information to the client chat field, that it is no longer connected
    void RemoveConnection(Connection::ChatClient* client);

private:
    /// Process message for the current chat
    void ProcessMessage(QString& timeUserString, QString& messageString);

private slots:
    /// Enables/Disables send button
    void EnableSendButton();
    /// Opens settings dialog
    void ShowSettingsDialog();
    /// Opens connection dialog
    void ShowConnectionDialog();
    /// Accepts pending connection
    void AcceptConnection();
    /// Shows chat history for specific item
    void ShowChatFor(QListWidgetItem* item);
    /// Sends message packet to the current chat host
    void SendMessage();
    /// Disconnects from current chat host
    void RemoveConnection();

public slots:
    /// Adds connection to the chat list
    void AddConnection(Connection::ChatClientItem* item);
};
#endif // MAINWINDOW_H
