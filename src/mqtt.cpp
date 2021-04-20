#include "mqtt.h"

mqtt_client::mqtt_client(){};
mqtt_client::~mqtt_client(){};

void mqtt_client::connect(std::string ADDRESS, std::string CLIENTID) {

    client = new mqtt::async_client(ADDRESS, CLIENTID, mqtt::create_options(MQTTVERSION_5));

    auto connOpts = mqtt::connect_options_builder().keep_alive_interval(std::chrono::seconds(1)).mqtt_version(MQTTVERSION_5).clean_start(true).finalize();

    try {
        client->connect(connOpts)->wait();
    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }

    client->set_connection_lost_handler([this](const std::string &) {
        this->client->reconnect()->wait();
        std::cout << "Recconecting" << std::endl;
    });

    client->set_message_callback([this](mqtt::const_message_ptr msg) { emit this->getMessage(QByteArray::fromStdString(msg->get_payload_str())); });
}

void mqtt_client::disconnect() {

    try {
        client->disconnect()->wait();
    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }
}

void mqtt_client::sendMessage(QByteArray msg) {}
