#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "app/appsettings.h"
#include "globaldefs.h"
#include "app/filemanager.h"

#include <QMessageBox>

extern AppSettings SETTINGS;
extern QString SETTINGS_PATH;

SettingsDialog::SettingsDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    portValidator(0, GlobalDefs::PortMaxValue)
{
    ui->setupUi(this);

    ui->UsernameTextEdit->setText(SETTINGS.Username);
    ui->UsernameTextEdit->setValidator(new QRegExpValidator(QRegExp("\\w{4,24}")));
    ui->PortField->setValidator(&portValidator);

    if (!SETTINGS.IsDefaultPort())
    {
        ui->IsDefaultPort->setChecked(false);
        ui->PortField->setText(QString::number(SETTINGS.Port));
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::EnablePort(bool checked)
{
    ui->PortField->setEnabled(!checked);
    if (checked)
    {
        ui->PortField->setText(QString::number(GlobalDefs::DefaultPort));
    }
}

void SettingsDialog::CheckPort()
{
    if (ui->PortField->text() != "" && !ui->PortField->hasAcceptableInput())
    {
        ui->PortField->setText(QString::number(GlobalDefs::PortMaxValue));
    }
}

void SettingsDialog::accept()
{
    if (!ui->UsernameTextEdit->hasAcceptableInput())
    {
        QMessageBox msg;
        msg.setWindowTitle("Invalid username");
        msg.setText(tr("Entered username is inacceptable:\n"
                       "1) it contains only latin characters, digits and underlines\n"
                       "2) its length is 4 to 24 characters."));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        return;
    }

    ushort newPort = ui->PortField->text().toUShort();

    if (((MainWindow*)parent())->HasConnections() && newPort != SETTINGS.Port)
    {
        QMessageBox msg;
        msg.setWindowTitle("Port change is unavailable");
        msg.setText(tr("Tou cannot change port while there are connected chat clients."));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        return;
    }

    SETTINGS.Username = ui->UsernameTextEdit->text();
    SETTINGS.Port = newPort;
    qLOG << "Saving new settings...";
    FileManager::SaveToXML<AppSettings>(SETTINGS, SETTINGS_PATH);
    ((MainWindow*)parent())->UpdateCurrentSessionInfo();
    QDialog::accept();
}
