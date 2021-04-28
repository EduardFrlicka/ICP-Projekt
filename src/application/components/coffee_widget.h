#ifndef COFFEE_WIDGET_H
#define COFFEE_WIDGET_H

#include "../mqtt.h"
#include "ui_coffee_widget.h"
#include <QDateTime>
#include <QSettings>
#include <QWidget>
class CoffeWidget : public QWidget, private Ui::CoffeWidget {
    Q_OBJECT

  public:
    explicit CoffeWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
    QString name;
    QString topic;
    QString widgetID;

  public slots:
    void addMessage(QByteArray msg, QString topicName);
    void on_set_btn_clicked();
    void on_delete_btn_clicked();
  signals:
    void sendMessage(QByteArray, QString, int my_message);
};
#endif
