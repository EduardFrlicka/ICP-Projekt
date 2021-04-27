#ifndef CAMERA_WIDGET_H
#define CAMERA_WIDGET_H

#include "../mqtt.h"
#include "image_window.h"
#include "ui_camera_widget.h"
#include <QDateTime>
#include <QWidget>
class CameraWidget : public QWidget, private Ui::CameraWidget {
    Q_OBJECT

  public:
    explicit CameraWidget(QWidget *parent, mqtt_client *client, QString name, QString topic);
    QString name;
    QString topic;
    QPixmap img;

  public slots:
    void addMessage(QByteArray msg, QString topicName);
    void on_set_btn_clicked();
    void on_delete_btn_clicked();
};
#endif
