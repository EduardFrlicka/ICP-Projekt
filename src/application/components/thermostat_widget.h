#ifndef THHERMOSTAT_WIDGET_H
#define THHERMOSTAT_WIDGET_H

#include "../mqtt.h"
#include "ui_thermostat_widget.h"
#include <QDateTime>
#include <QWidget>

class ThermostatWidget : public QWidget, private Ui::ThermostatWidget {
    Q_OBJECT

  public:
    explicit ThermostatWidget(QWidget *parent, mqtt_client *client, QString name, QString topic);
    QString name;
    QString topic;

  public slots:
    void addMessage(QByteArray msg, QString topicName);
    void on_set_btn_clicked();
    void on_delete_btn_clicked();
  signals:
    void sendMessage(QByteArray, QString, int my_message);
};
#endif
