#ifndef SERVER_DIALOG_H
#define SERVER_DIALOG_H

#include "ui_server_dialog.h"
#include <QDialog>

#define DEFAULT_ADRESS "tcp://broker.emqx.io:1883"

class ServerDialog : public QDialog, private Ui::ServerDialog {
    Q_OBJECT

  public:
    ServerDialog(QWidget *parent = nullptr);
    QString getServerAdress();
    QString getClientId();
};
#endif
