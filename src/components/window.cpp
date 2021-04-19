#include "window.h"

Mqtt_explorer::Mqtt_explorer(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    ServerDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        this->clientName = dialog.clientName;
        this->serverAdress = dialog.serverAdress;
        client.create(serverAdress.toStdString(), clientName.toStdString());
    }
}

// SLOTS

void Mqtt_explorer::on_send_btn_clicked() {
    QListWidgetItem *item = new QListWidgetItem(textEdit->toPlainText());
    listWidget->addItem(item);
    listWidget->scrollToBottom();
    textEdit->setText("");
}

void Mqtt_explorer::on_attachFile_btn_clicked() {
    QStringList fileNames =
    QFileDialog::getOpenFileNames(this, tr("Attach Image"), "./",
                                      tr("Images (*.png *.xpm *.jpg *.jpeg)"));

    if (fileNames.size() > 0) {
        QString url = fileNames[0];
        QPixmap img(url);
        ImageForm *image = new ImageForm();
        image->setFixedHeight(img.height());
        image->setFixedWidth(img.width());
        image->SetImage(&img);
        image->show();
    }
}

void Mqtt_explorer::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {}

// CLASS FUNCTIONS

void Mqtt_explorer::addMessage(QString msg) { listWidget->addItem(msg); }
