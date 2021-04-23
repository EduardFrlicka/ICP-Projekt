#include "mqtt.h"

mqtt_client::mqtt_client() {
}
mqtt_client::~mqtt_client() {
}

void mqtt_client::connect(std::string ADDRESS, std::string CLIENTID) {

    client = new mqtt::async_client(ADDRESS, CLIENTID, mqtt::create_options(MQTTVERSION_5));

    auto connOpts = mqtt::connect_options_builder().keep_alive_interval(std::chrono::seconds(10)).mqtt_version(MQTTVERSION_5).clean_start(true).finalize();

    try {
        client->connect(connOpts)->wait();
    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }

    client->set_connection_lost_handler([this](const std::string &) { this->client->reconnect()->wait(); });

    client->set_message_callback([this](mqtt::const_message_ptr msg) { emit this->getMessage(QByteArray::fromStdString(msg->get_payload_str())); });

    this->subscribe("test");
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

void mqtt_client::sendMessage(std::string topic, QByteArray msg) {
    try {
        this->client->publish(topic, msg.toStdString());
    } catch (const mqtt::exception &exc) {
        std::cout << exc.what() << std::endl;
    }
}

int mqtt_client::subscribe(std::string topic) {
    try {
        auto subOpts = mqtt::subscribe_options(NO_LOCAL);
        auto tok = this->client->subscribe(topic, QOS, subOpts);
        tok->wait();
        return tok->get_return_code();
    } catch (const mqtt::exception &exc) {
        std::cout << exc.what() << std::endl;
        return 1;
    }
}