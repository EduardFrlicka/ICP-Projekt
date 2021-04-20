#include "server_dialog.h"

ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    serverAdressInput->setText(DEFAULT_ADRESS);
}

QString ServerDialog::getServerAdress() { return serverAdressInput->text(); }

QString ServerDialog::getClientId() { return clientNameInput->text(); }
