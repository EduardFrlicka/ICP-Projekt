#ifndef THERMOMETER_WIDGET_H
#define THERMOMETER_WIDGET_H

#include "../mqtt.h"
#include "ui_thermometer_widget.h"
#include <QDateTime>
#include <QWidget>
class ThermometerWidget : public QWidget, private Ui::ThermometerWidget {
    Q_OBJECT

  public:
    explicit ThermometerWidget(QWidget *parent, mqtt_client *client, QString name, QString topic);
    QString name;
    QString topic;
    QByteArray state = "off";

  public slots:
    void addMessage(QByteArray msg, QString topicName);
    void on_delete_btn_clicked();
};
#endif
