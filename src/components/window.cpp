#include "window.h"

Mqtt_explorer::Mqtt_explorer(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    ServerDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        this->clientName = dialog.clientName;
        this->serverAdress = dialog.serverAdress;
        client.create(serverAdress.toStdString(), clientName.toStdString());
    }
    connect(&client, SIGNAL(getMessage(QByteArray)), this,
            SLOT(addMessage(QByteArray)));
}

// SLOTS

void Mqtt_explorer::on_send_btn_clicked() {
    QListWidgetItem *item = new QListWidgetItem(textEdit->toPlainText());
    listWidget->scrollToBottom();
    QByteArray msg =
        QByteArray::fromStdString(textEdit->toPlainText().toStdString());
    msg.append((char)1);

    client.sendMsg(msg);
    textEdit->setText("");
}

void Mqtt_explorer::on_attachFile_btn_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this, tr("Attach Image"), "./", tr("Images (*.png *.jpg *.jpeg)"));

    if (fileNames.size() > 0) {
        QString url = fileNames[0];
        QImage img(url);

        ifstream myfile(fileNames[0].toStdString());
        QByteArray msg;
        char byte = 0;
        while (myfile.get(byte)) {
            msg.append(byte);
        }
        msg.append((char)0);

        client.sendMsg(msg);
    }
}

void Mqtt_explorer::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {

    if (item->data(Qt::UserRole).isNull()) {
        std::cout << "Nema žiadne data v sebe" << std::endl;
        return;
    }

    ImageForm *image = new ImageForm();
    QPixmap img;
    img.loadFromData(item->data(Qt::UserRole).value<QByteArray>());
    std::cout << img.height() << std::endl;
    image->SetImage(&img);
    image->show();
}

// CLASS FUNCTIONS

void Mqtt_explorer::addMessage(QByteArray msg) {
    QListWidgetItem *item = new QListWidgetItem();
    if (msg.back() == (char)0) {
        msg.chop(1);
        item->setData(Qt::DisplayRole, "[ image file ]");
        item->setData(Qt::UserRole, msg);
    } else {
        msg.chop(1);
        item->setData(Qt::DisplayRole, msg);
    }

    listWidget->addItem(item);
}
