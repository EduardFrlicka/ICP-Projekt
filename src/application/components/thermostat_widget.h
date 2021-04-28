
#ifndef THHERMOSTAT_WIDGET_H
#define THHERMOSTAT_WIDGET_H

#include "../mqtt.h"
#include "ui_thermostat_widget.h"
#include <QDateTime>
#include <QSettings>
#include <QWidget>

class ThermostatWidget : public QWidget, private Ui::ThermostatWidget {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Coffe Machine Widget object
     *
     * @param parent parent object
     * @param client active mqtt connection client
     * @param name name of device
     * @param topic topic to interact with. (mqtt topic)
     * @param widgetID
     */
    explicit ThermostatWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
    QString name;
    QString topic;
    QString widgetID;

  public slots:
    /**
     * @brief New message arrived handling.
     *
     * Checks new message. If subscribed topic is same as widget topic message is accepted.
     * Sets status label text from arrived message.
     *
     * @param msg message payload
     * @param topicName
     */
    void addMessage(QByteArray msg, QString topicName);

    /**
     * @brief emit sendMessage singal to send data from inputBox to thermostat
     *
     */
    void on_set_btn_clicked();

    /**
     * @brief Delete widget from layout and delete from config file.
     *
     */
    void on_delete_btn_clicked();

  signals:
    void sendMessage(QByteArray, QString, int my_message);
};
#endif
