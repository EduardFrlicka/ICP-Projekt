#include "thermostat_widget.h"

ThermostatWidget::ThermostatWidget(QWidget *parent, mqtt_client *client, QString name, QString topic) : QWidget(parent) {
    setupUi(this);
    this->name = name;
    this->topic = topic;

    this->name_text->setText(name);
    this->topic_text->setText(topic);

    QPixmap p(":/thermostat.png");
    int w = this->icon_label->width();
    int h = this->icon_label->height();
    this->icon_label->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));

    connect(client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
    connect(this, SIGNAL(sendMessage(QByteArray, QString, int)), parent, SLOT(addMessage(QByteArray, QString, int)));
}

void ThermostatWidget::addMessage(QByteArray msg, QString topicName) {
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentTime = dateTime.toString("[HH:mm:ss] ");
    if (topicName == this->topic) {
        this->status_text->setText(currentTime + msg);
    }
}

void ThermostatWidget::on_set_btn_clicked() {
    emit this->sendMessage(QByteArray::fromStdString(std::to_string(this->temperature_input->value())), this->topic, 1);
}

void ThermostatWidget::on_delete_btn_clicked() {
    delete this;
}