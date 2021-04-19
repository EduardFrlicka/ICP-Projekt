#include "window.h"
#include <QWidget>

Mqtt_explorer::Mqtt_explorer(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    ServerDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        this->clientName = dialog.clientName;
        this->serverAdress = dialog.serverAdress;
        client.create(serverAdress.toStdString(), clientName.toStdString());
    }
}

void Mqtt_explorer::on_send_btn_clicked() {
    QListWidgetItem *item = new QListWidgetItem(textEdit->toPlainText());
    listWidget->addItem(item);
    listWidget->scrollToBottom();
    textEdit->setText("");
}

void Mqtt_explorer::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {}