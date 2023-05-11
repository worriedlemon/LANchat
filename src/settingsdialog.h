#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui
{
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    const QIntValidator portValidator;
    Ui::SettingsDialog *ui;
public:
    explicit SettingsDialog(MainWindow *parent = nullptr);
    ~SettingsDialog();

private slots:
    void EnablePort(bool checked);
    void CheckPort();

public slots:
    void accept() override;
};

#endif // SETTINGSDIALOG_H
