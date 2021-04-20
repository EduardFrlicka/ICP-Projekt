#include "window.h"

Mqtt_explorer::Mqtt_explorer(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    ServerDialog dialog;

    auto dialog_ret = dialog.exec();
    if (dialog_ret == QDialog::Accepted) {
        client.connect(dialog.getServerAdress().toStdString(), dialog.getClientId().toStdString());
    }
    if (dialog_ret == QDialog::Rejected) {
        exit(0);
    }

    connect(&client, SIGNAL(getMessage(QByteArray)), this, SLOT(addMessage(QByteArray)));
}

// SLOTS

void Mqtt_explorer::on_send_btn_clicked() {
    auto message_data = textEdit->toPlainText().toStdString();

    if (message_data.size() == 0)
        return;

    QByteArray msg = QByteArray::fromStdString(message_data);
    msg.append(STRING_MSG);

    client.sendMessage(msg);
    this->addMessage(msg, 1);

    textEdit->setText("");
}

void Mqtt_explorer::on_attachFile_btn_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Attach Image"), "./", tr("Images (*.png *.jpg *.jpeg)"));

    if (fileNames.size() == 0)
        return;

    QByteArray msg;
    // open file
    std::ifstream myfile(fileNames[0].toStdString());

    // write image data to message
    char byte = 0;
    while (myfile.get(byte)) {
        msg.append(byte);
    }
    // append byte to recognize type of message - image
    msg.append(IMAGE_MSG);

    client.sendMessage(msg);
    this->addMessage(msg, 1);
}

void Mqtt_explorer::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {

    if (item->data(Qt::UserRole).isNull())
        return;

    // load image from item data
    QPixmap img;
    img.loadFromData(item->data(Qt::UserRole).value<QByteArray>());

    // show image
    ImageForm *image = new ImageForm();
    image->SetImage(&img);
    image->show();
}

// CLASS FUNCTIONS

void Mqtt_explorer::addMessage(QByteArray msg, int myMessage = 0) {
    QListWidgetItem *item = new QListWidgetItem();
    QIcon icon = QIcon("./img/person.ico");
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsEnabled);

    if (myMessage) {
        item->setBackground(Qt::gray);
    }

    int msg_type = msg.back();
    msg.chop(1);

    switch (msg_type) {
    case IMAGE_MSG:
        item->setData(Qt::DisplayRole, "[ image file ]");
        item->setData(Qt::UserRole, msg);
        break;
    case STRING_MSG:
        item->setData(Qt::DisplayRole, msg);
        break;
    default:
        break;
        return;
    }

    // Add item and scroll down
    listWidget->addItem(item);
    listWidget->scrollToBottom();
}