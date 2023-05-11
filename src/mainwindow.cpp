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

#include <QtConcurrent>

extern AppSettings SETTINGS;

MainWindow::MainWindow()
    : QMainWindow(nullptr),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(GlobalDefs::GetApplicationTitle());
    UpdateCurrentSessionInfo();

    listener.listen(QHostAddress::Any, SETTINGS.Port);
    connect(&listener, SIGNAL(newConnection()), this, SLOT(AcceptConnection()));
}

MainWindow::~MainWindow()
{
    listener.close();
    delete ui;
}

void MainWindow::AcceptConnection()
{
    ChatClient* client = new ChatClient(listener.nextPendingConnection(), *this);
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

void MainWindow::ShowChatFor(QListWidgetItem* item)
{
    if (!ui->ChatWindow->isEnabled()) ui->ChatWindow->setEnabled(true);

    ui->MessageArea->clear();
    qLOG << "Showing chat window for " << item->text();
    for (QString const & text : history[((ChatClientItem*)item)->GetLink()])
        ui->MessageArea->addItem(text);
}

QListWidget& MainWindow::GetChatList() const
{
    return *ui->ChatList;
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

    ChatClient* current = ((ChatClientItem*)ui->ChatList->currentItem())->GetLink();
    if (client == nullptr) client = current;

    history[client].append(timeUserString);
    history[client].append(messageString);
    if (client == current) ProcessMessage(timeUserString, messageString);
}

void MainWindow::SendMessage()
{
    if (!ui->SendButton->isEnabled()) return;

    ChatClient* current = ((ChatClientItem*)ui->ChatList->currentItem())->GetLink();
    QString msg = ui->MessageTextEdit->text();
    MessagePacket pkg(msg);
    current->SendPacket(pkg);
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
