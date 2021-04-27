#ifndef MQTT_H
#define MQTT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "mqtt/async_client.h"


#include <QMessageBox>
#include <QUuid>

#define NO_LOCAL true
#define QOS 1

class mqtt_client : public QWidget {
    Q_OBJECT

  public:
    mqtt_client();
    ~mqtt_client();
    void connect(std::string ADDRESS);
    void disconnect();

    int subscribe(std::string topic);
    int unsubscribe(std::string topic);
    void sendMessage(QByteArray msg);
    void setCurrentTopic(std::string topic);
    mqtt::async_client *client;
    std::string currentTopic;
  signals:
    void getMessage(QByteArray msg, QString topicName);
    void sendStatusText(QString msg);
};

#endif
