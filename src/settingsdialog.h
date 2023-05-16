#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui
{
    class SettingsDialog;
}

/// Simple dialog for changing session information (program settings)
class SettingsDialog : public QDialog
{
    Q_OBJECT

    const QIntValidator portValidator;
    Ui::SettingsDialog *ui;
public:
    explicit SettingsDialog(MainWindow *parent = nullptr);
    ~SettingsDialog();

private slots:
    /// Enables and disables port field depending on 'Default Port' checked value
    void EnablePort(bool checked);
    /// Validating port field
    void CheckPort();

public slots:
    /// Parses given settings and save it to file
    void accept() override;
};

#endif // SETTINGSDIALOG_H
