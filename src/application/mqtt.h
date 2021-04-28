/**
 * @file mqtt.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief MQTT class, wrapper for mqtt::async_client
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef MQTT_H
#define MQTT_H

#include "mqtt/async_client.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <QMessageBox>
#include <QUuid>

#define NO_LOCAL true
#define QOS 0

class mqtt_client : public QWidget {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new mqtt client object
     *
     */
    mqtt_client();

    /**
     * @brief Destroy the mqtt client object
     *
     */
    ~mqtt_client();

    /**
     * @brief Create connection with server
     * Creates new async_client and connect him to server. Sets options for connection.
     * Sets callback function for client.
     *
     * @param ADDRESS server adress
     */
    void connect(std::string ADDRESS);

    /**
     * @brief Disconnect client from server
     *
     */
    void disconnect();

    /**
     * @brief Subscrie topic
     *
     * @param topic topic name
     * @return 0 sucess
     * @return 1 failed
     */
    int subscribe(std::string topic);

    /**
     * @brief Unsubscribe topic
     *
     * @param topic topic name
     * @return 0 sucess
     * @return 1 failed
     */
    int unsubscribe(std::string topic);

    /**
     * @brief Send message to server
     *
     * @param msg data payload
     */
    void sendMessage(QByteArray msg);

    /**
     * @brief Set current topic for message publising
     *
     * @param topic topic name
     */
    void setCurrentTopic(std::string topic);

    mqtt::async_client *client;
    std::string currentTopic;
  signals:
    /**
     * @brief signal thats is triggered when new message arrived.
     *
     * @param msg data payload
     * @param topicName topic name
     */
    void getMessage(QByteArray msg, QString topicName);

    /**
     * @brief Sends signal to main application to set status bar text
     *
     * @param msg message to be displayed
     */
    void sendStatusText(QString msg);
};

#endif
