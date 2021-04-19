#ifndef DIALOG_H
#define DIALOG_H

#include "ui_server_dialog.h"

class ServerDialog : public QDialog, private Ui::ServerDialog {
    Q_OBJECT

  public:
    ServerDialog(QWidget *parent = nullptr);
    QString clientName;
    QString serverAdress;

  public slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
#endif
