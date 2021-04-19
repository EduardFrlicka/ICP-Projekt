#ifndef MQTT_H
#define MQTT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "mqtt/async_client.h"
#include "mqtt/client.h"
#include "mqtt/topic.h"

#include <QMessageBox>

using namespace std;
class mqtt_client : public QWidget {
    Q_OBJECT

  public:
    mqtt_client();
    ~mqtt_client();
    void create(string ADDRESS, string CLIENTID);
    void disconnect();
    void sendMsg(std::string msg);

    mqtt::async_client *client;
    mqtt::topic *topic;

  signals:
    void getMessage(QString message);
};

#endif
