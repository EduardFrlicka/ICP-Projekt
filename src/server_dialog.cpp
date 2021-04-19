#include "window.h"
#include <QDialog>

ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    serverAdressInput->setText("tcp://test.mosquitto.org:1883");
}

void ServerDialog::on_buttonBox_accepted() {
    this->clientName = clientNameInput->text();
    this->serverAdress = serverAdressInput->text();
}
void ServerDialog::on_buttonBox_rejected() { exit(0); }