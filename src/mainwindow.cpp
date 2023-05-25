#include "app/appsettings.h"
#include "connection/client.h"
#include "connection/messagetimepair.h"
#include "connection/messagepacket.h"
#include "connection/informationpacket.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsdialog.h"
#include "connectiondialog.h"

using namespace Connection;

#include <QDateTime>

extern AppSettings SETTINGS;

MainWindow::MainWindow()
    : QMainWindow(nullptr),
      ui(new Ui::MainWindow),
      currentChat(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(GlobalDefs::GetApplicationTitle());
    UpdateCurrentSessionInfo();

    listener.listen(QHostAddress::Any, SETTINGS.Port);
    connect(&listener, SIGNAL(newConnection()), this, SLOT(AcceptConnection()));
}

MainWindow::~MainWindow()
{
    history.clear();
    listener.close();
    delete ui;
}

void MainWindow::AcceptConnection()
{
    ChatClient* client = new ChatClient(listener.nextPendingConnection(), *this, ChatClient::ChatType::INCOMING);
    ChatClientItem* item = new ChatClientItem(client);
    qLOG << "Listener caught pending connection: " << item->text();
    AddConnection(item);
}

void MainWindow::UpdateCurrentSessionInfo()
{
    ui->CurrentSessionInfo->setText("Current session: " + SETTINGS.ToString());
    InformationPacket information(SETTINGS);
    for (ChatClient* chat : history.keys()) chat->SendPacket(information);
}

void MainWindow::EnableSendButton()
{
    ui->SendButton->setEnabled(ui->MessageTextEdit->text() != "");
}

void MainWindow::ShowSettingsDialog()
{
    SettingsDialog* dialog = new SettingsDialog(this);
    dialog->show();
}

void MainWindow::ShowConnectionDialog()
{
    ConnectionDialog* dialog = new ConnectionDialog(this);
    dialog->show();
}

void MainWindow::AddConnection(ChatClientItem* item)
{
    history.insert(item->GetLink(), *new QList<QString>());
    ui->ChatList->addItem(item);
}

void MainWindow::RemoveConnection()
{
    ui->ChatList->removeItemWidget(currentChat);
    ChatClient* chat = currentChat->GetLink();
    history.remove(chat);
    delete chat;
    delete currentChat;
    currentChat = nullptr;
    ui->MessageArea->clear();
    ui->DisconnectButton->setEnabled(false);
    ui->MessageTextEdit->setEnabled(false);
    ui->ChatList->clearSelection();
}

void MainWindow::RemoveConnection(ChatClient* client)
{
    QList<QListWidgetItem*> foundItems = ui->ChatList->findItems(ChatClientItem(client).text(), Qt::MatchExactly);
    QListWidgetItem* found = foundItems[0];
    ChatClient* chat = ((ChatClientItem*)found)->GetLink();
    QString timeUserString = tr("[_PROGRAM] at ") + QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy");
    QString messageString = tr("-------- Client ") + found->text() + tr(" disconnected --------");
    history[chat].append(timeUserString);
    history[chat].append(messageString);
    if (currentChat != nullptr && chat == currentChat->GetLink())
        ProcessMessage(timeUserString, messageString);
}

void MainWindow::ShowChatFor(QListWidgetItem* item)
{
    if (currentChat == nullptr)
    {
        ui->DisconnectButton->setEnabled(true);
        ui->MessageTextEdit->setEnabled(true);
    }

    currentChat = (ChatClientItem*)item;

    ui->MessageArea->clear();
    qLOG << "Showing chat window for " << item->text();
    for (QString const & text : history[currentChat->GetLink()])
        ui->MessageArea->addItem(text);
}

void MainWindow::ProcessMessage(QString& timeUserString, QString& messageString)
{
    ui->MessageArea->addItem(timeUserString);
    ui->MessageArea->addItem(messageString);
}

void MainWindow::ProcessMessage(MessageTimePair const & pair, ChatClient* client)
{
    QString timeUserString = "[" + (client == nullptr ? SETTINGS.Username : client->Name) + "]" +
            tr(" at ") + QDateTime::fromSecsSinceEpoch(pair.Time).toString("hh:mm:ss dd/MM/yyyy");
    QString messageString = pair.Message;

    if (client == nullptr) client = currentChat->GetLink();

    history[client].append(timeUserString);
    history[client].append(messageString);
    if (currentChat != nullptr && client == currentChat->GetLink())
        ProcessMessage(timeUserString, messageString);
}

bool MainWindow::HasConnections() const
{
    return ui->ChatList->children().count() != 0;
}

void MainWindow::SendMessage()
{
    if (!ui->SendButton->isEnabled()) return;

    QString msg = ui->MessageTextEdit->text();
    MessagePacket pkg(msg);
    currentChat->GetLink()->SendPacket(pkg);
    ProcessMessage(MessageTimePair(msg, QDateTime::currentSecsSinceEpoch()));
    ui->MessageTextEdit->clear();
}

void MainWindow::UpdateClientInfo(ChatClient& client, AppSettings& information)
{
    QList<QListWidgetItem*> found = ui->ChatList->findItems(ChatClientItem(&client).text(), Qt::MatchExactly);
    client.Name = information.Username;
    ((ChatClientItem*)found[0])->UpdateText();
    ui->ChatList->update();
}
