#include "camera_widget.h"

CameraWidget::CameraWidget(QWidget *parent, mqtt_client *client, QString name, QString topic) : QWidget(parent) {
    setupUi(this);
    this->name = name;
    this->topic = topic;

    this->name_text->setText(name);
    this->topic_text->setText(topic);

    connect(client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
}

void CameraWidget::addMessage(QByteArray msg, QString topicName) {
    if (topicName == this->topic) {
        QPixmap img;
        img.loadFromData(msg);
        QDateTime dateTime = dateTime.currentDateTime();
        QString currentTime = dateTime.toString("[HH:mm:ss] ");

        if (!img.toImage().isNull()) {
            this->status_text->setText(currentTime + "image arrived");
            this->img.loadFromData(msg);
        } else {
            this->status_text->setText(currentTime + "unknown data");
        }
    }
}

void CameraWidget::on_set_btn_clicked() {
    if (!this->img.toImage().isNull()) {
        ImageForm *image = new ImageForm();
        image->SetImage(&img);
        image->show();
    }
}

void CameraWidget::on_delete_btn_clicked() {
    delete this;
}