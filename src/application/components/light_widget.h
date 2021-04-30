/**
 * @file light_widget.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for Light Widget. Input handling and internal setup.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
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
    /**
     * @brief Construct a new Light Widget object
     *
     * @param parent parent object
     * @param client active mqtt connection client
     * @param name name of device
     * @param topic topic to interact with. (mqtt topic)
     * @param widgetID
     */
    explicit LightWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
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
     * @brief Switchs lights on/off. Toggle base.
     * Toogle lights. States is selected from 'state' class variable.
     * Function emit signal sendMessage to main window.
     *
     */
    void on_switch_btn_clicked();

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
    void sendMessage(QByteArray, QString, int my_message);
    void widget_deleted_signal(QString topicName);
};
#endif
