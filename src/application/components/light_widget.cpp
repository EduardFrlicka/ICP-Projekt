#include "light_widget.h"

LightWidget::LightWidget(QWidget *parent, mqtt_client *client, QString name, QString topic) : QWidget(parent) {
    setupUi(this);
    this->name = name;
    this->topic = topic;

    this->name_text->setText(name);
    this->topic_text->setText(topic);

    QPixmap p(":/lightbulb.png");
    int w = this->icon_label->width();
    int h = this->icon_label->height();
    this->icon_label->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));

    connect(client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
    connect(this, SIGNAL(sendMessage(QByteArray, QString, int)), parent, SLOT(addMessage(QByteArray, QString, int)));
}

void LightWidget::addMessage(QByteArray msg, QString topicName) {
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentTime = dateTime.toString("[HH:mm:ss] ");
    if (topicName == this->topic) {
        this->status_text->setText(currentTime + msg);
    }
}

void LightWidget::on_switch_btn_clicked() {
    if (this->state == "off") {
        this->state = "on";
    } else {
        this->state = "off";
    }
    emit this->sendMessage(this->state, this->topic, 1);
}

void LightWidget::on_delete_btn_clicked() {
    delete this;
}