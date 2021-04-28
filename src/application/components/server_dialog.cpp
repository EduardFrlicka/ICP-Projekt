/**
 * @file server_dialog.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "server_dialog.h"

ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);

    // sets default text for server adress input. Only for test purpose.
    serverAdressInput->setText(DEFAULT_ADRESS);
}

QString ServerDialog::getServerAdress() {
    return serverAdressInput->text();
}
