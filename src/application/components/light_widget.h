#ifndef LIGHT_WIDGET_H
#define LIGHT_WIDGET_H

#include "../mqtt.h"
#include "ui_light_widget.h"
#include <QDateTime>
#include <QSettings>
#include <QWidget>
class LightWidget : public QWidget, private Ui::LightWidget {
    Q_OBJECT

  public:
    explicit LightWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
    QString name;
    QString topic;
    QString widgetID;
    QByteArray state = "off";

  public slots:
    void addMessage(QByteArray msg, QString topicName);
    void on_switch_btn_clicked();
    void on_delete_btn_clicked();
  signals:
    void sendMessage(QByteArray, QString, int my_message);
};
#endif
