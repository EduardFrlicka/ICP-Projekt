/**
 * @file thermometer_widget.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for Thermometer Widget. Input handling and internal setup.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef THERMOMETER_WIDGET_H
#define THERMOMETER_WIDGET_H

#include "../mqtt.h"
#include "ui_thermometer_widget.h"
#include <QDateTime>
#include <QSettings>
#include <QWidget>

class ThermometerWidget : public QWidget, private Ui::ThermometerWidget {
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
    explicit ThermometerWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
    QString name;
    QString topic;
    QString widgetID;
    QByteArray state = "off";

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
     * @brief Handle unsubscribe signal from main application
     * If topicName is same as widget topic, delete widget
     *
     * @param topicName
     */
    void handle_unsubscribe(QString topicName);

    /**
     * @brief Handle delete button click
     * Calls deleteWidget() function
     *
     */
    void on_delete_btn_clicked();

    /**
     * @brief Delete widget from layout and delete from config file.
     *
     */
    void deleteWidget();
  signals:
    void widget_deleted_signal(QString topicName);
};
#endif
