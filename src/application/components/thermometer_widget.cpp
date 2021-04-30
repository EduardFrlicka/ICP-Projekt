/**
 * @file thermometer_widget.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "thermometer_widget.h"

ThermometerWidget::ThermometerWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID) : QWidget(parent) {
    setupUi(this);

    // asign variables
    this->name = name;
    this->topic = topic;
    this->widgetID = widgetID;
    this->name_text->setText(name);
    this->topic_text->setText(topic);

    // set default widget icon
    QPixmap p(":/thermometer.png");
    int w = this->icon_label->width();
    int h = this->icon_label->height();
    this->icon_label->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));

    // connect signal, when new message arrive, function addMessage is called
    connect(client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
    // connect signal, delete widget when topic is unsubscribed from explorer
    connect(parent, SIGNAL(unsubscribe_signal(QString)), this, SLOT(handle_unsubscribe(QString)));
    // connect signal, unsubscribe topic from explorer when widget is deleted
    connect(this, SIGNAL(widget_deleted_signal(QString)), parent, SLOT(unsubscribe_topic(QString)));
}

void ThermometerWidget::addMessage(QByteArray msg, QString topicName) {
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentTime = dateTime.toString("[HH:mm:ss] ");
    // check if message topic is same as widget topic
    if (topicName == this->topic) {
        this->status_text->setText(currentTime + msg);
    }
}

void ThermometerWidget::handle_unsubscribe(QString topicName) {
    if (topicName == this->topic) {
        this->deleteWidget();
    }
}

void ThermometerWidget::on_delete_btn_clicked() {
    emit this->widget_deleted_signal(this->topic);
    deleteWidget();
}

void ThermometerWidget::deleteWidget() {
    QSettings settings;
    settings.beginGroup("widget" + this->widgetID);
    settings.remove("");
    delete this;
}