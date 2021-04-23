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

    void chooseTopic(std::string name);
    void sendMessage(QByteArray msg);

    mqtt::async_client *client;
    mqtt::topic *topic = nullptr;
  signals:
    void getMessage(QByteArray msg);
};

#endif
