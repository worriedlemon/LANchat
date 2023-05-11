#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "globaldefs.h"
#include "connection/client.h"

#include "connection/informationpacket.h"
#include "connection/requestpacket.h"

#include <QHostAddress>
#include <QMessageBox>

extern AppSettings SETTINGS;

ConnectionDialog::ConnectionDialog(MainWindow* parent)
    : QDialog(parent),
      ui(new Ui::ConnectionDialog()),
      ipSegmentValidator(0, 255),
      portValidator(0, GlobalDefs::PortMaxValue)
{
    ui->setupUi(this);

    ui->IPSegment1->setValidator(&ipSegmentValidator);
    ui->IPSegment2->setValidator(&ipSegmentValidator);
    ui->IPSegment3->setValidator(&ipSegmentValidator);
    ui->IPSegment4->setValidator(&ipSegmentValidator);
    ui->PortSegment->setValidator(&portValidator);

    connect(this, SIGNAL(ConnectionReady(Connection::ChatClientItem*)), parent, SLOT(AddConnection(Connection::ChatClientItem*)));
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

bool ConnectionDialog::CheckIPBytes()
{
    return ui->IPSegment1->hasAcceptableInput() &&
            ui->IPSegment2->hasAcceptableInput() &&
            ui->IPSegment3->hasAcceptableInput() &&
            ui->IPSegment4->hasAcceptableInput();
}

void ConnectionDialog::EnablePort(bool checked)
{
    ui->PortSegment->setEnabled(!checked);
    if (checked)
    {
        ui->PortSegment->setText(QString::number(GlobalDefs::DefaultPort));
    }
}

void ConnectionDialog::CheckPort()
{
    QString input = ui->PortSegment->text();
    ipSegmentValidator.fixup(input);
    ui->PortSegment->setText(input);
}

void ConnectionDialog::accept()
{
    if (!CheckIPBytes())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("IP address is invalid");
        msgBox.setText(tr("IP address is invalid.\nAll bytes have to have value between 0 and 255."));
        msgBox.exec();
        return;
    }

    QHostAddress address((byte)ui->IPSegment1->text().toUInt() << 24 |
                         (byte)ui->IPSegment2->text().toUInt() << 16 |
                         (byte)ui->IPSegment3->text().toUInt() << 8 |
                         (byte)ui->IPSegment4->text().toUInt());
    quint16 port = ui->PortSegment->text().toUShort();
    qLOG << "Parsed address: " << address.toString() << ":" << port;

    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(address, port);
    if (!socket->waitForConnected(10000))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Connection error");
        msgBox.setText(tr("Cannot connect to specified host.\nPlease, check your connection settings."));
        msgBox.exec();
        delete socket;
        return;
    }
    Connection::ChatClient* client = new Connection::ChatClient(socket, (MainWindow&)*parent());
    emit ConnectionReady(new Connection::ChatClientItem(client));

    Connection::RequestPacket requestForInformation;
    Connection::InformationPacket information(SETTINGS);
    client->SendPacket(requestForInformation);
    client->SendPacket(information);

    QDialog::accept();
    delete this;
}
