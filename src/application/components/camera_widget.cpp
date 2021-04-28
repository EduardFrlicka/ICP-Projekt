/**
 * @file camera_widget.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "camera_widget.h"

CameraWidget::CameraWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID) : QWidget(parent) {
    setupUi(this);

    // asign variables
    this->name = name;
    this->topic = topic;
    this->widgetID = widgetID;
    this->name_text->setText(name);
    this->topic_text->setText(topic);

    // set default widget icon
    QPixmap p(":/camera.png");
    int w = this->icon_label->width();
    int h = this->icon_label->height();
    this->icon_label->setPixmap(p.scaled(w, h, Qt::KeepAspectRatio));

    // connect signal, when new message arrive, function addMessage is called
    connect(client, SIGNAL(getMessage(QByteArray, QString)), this, SLOT(addMessage(QByteArray, QString)));
}

void CameraWidget::addMessage(QByteArray msg, QString topicName) {
    // check if message topic is same as widget topic
    if (topicName == this->topic) {
        QPixmap img;
        img.loadFromData(msg);

        QDateTime dateTime = dateTime.currentDateTime();
        QString currentTime = dateTime.toString("[HH:mm:ss] ");

        // try parse check
        if (!img.toImage().isNull()) {
            this->status_text->setText(currentTime + "image arrived");
            this->img.loadFromData(msg);
        } else {
            this->status_text->setText(currentTime + "unknown data");
        }
    }
}

void CameraWidget::on_show_btn_clicked() {
    if (!this->img.toImage().isNull()) {
        ImageForm *image = new ImageForm();
        image->SetImage(&img);
        image->show();
    }
}

void CameraWidget::on_delete_btn_clicked() {
    QSettings settings;
    settings.beginGroup("widget" + widgetID);
    settings.remove("");
    delete this;
}