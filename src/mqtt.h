#ifndef MQTT_H
#define MQTT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "mqtt/async_client.h"
#include "mqtt/client.h"
#include "mqtt/topic.h"

#include <QMessageBox>

#define NO_LOCAL true
#define QOS 1

class mqtt_client : public QWidget {
    Q_OBJECT

  public:
    mqtt_client();
    ~mqtt_client();
    void connect(std::string ADDRESS, std::string CLIENTID);
    void disconnect();

    int mqtt_client::subscribe(std::string topic);
    void sendMessage(QByteArray msg);
    void setCurrentTopic(std::string topic);
    mqtt::async_client *client;
  signals:
    void getMessage(QByteArray msg);

  private:
    std::string currentTopic;
};

#endif
