/**
 * @file mqtt.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "mqtt.h"

mqtt_client::mqtt_client() {
}
mqtt_client::~mqtt_client() {
}

void mqtt_client::connect(std::string ADDRESS) {

    client = new mqtt::async_client(ADDRESS, QUuid::createUuid().toString().toStdString(), mqtt::create_options(MQTTVERSION_5));

    auto connOpts = mqtt::connect_options_builder().keep_alive_interval(std::chrono::seconds(3600)).mqtt_version(MQTTVERSION_5).clean_start(true).connect_timeout(std::chrono::seconds(5)).finalize();

    try {
        client->connect(connOpts)->wait();
    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }

    client->set_message_callback([this](mqtt::const_message_ptr msg) { emit this->getMessage(QByteArray::fromStdString(msg->get_payload_str()), QByteArray::fromStdString(msg->get_topic())); });
    client->set_connection_lost_handler([](const std::string &) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Connection lost restart application!");
        messageBox.setFixedSize(500, 200);
        exit(1);
    });
}

void mqtt_client::sendMessage(QByteArray msg) {
    try {
        this->client->publish(this->currentTopic, msg.toStdString());
    } catch (const mqtt::exception &exc) {
        emit this->sendStatusText(exc.what());
    }
}

int mqtt_client::subscribe(std::string topic) {
    try {
        auto subOpts = mqtt::subscribe_options(NO_LOCAL);
        auto tok = this->client->subscribe(topic, QOS, subOpts);
        tok->wait();
        return tok->get_return_code();
    } catch (const mqtt::exception &exc) {
        emit this->sendStatusText(exc.what());
        return 1;
    }
}

int mqtt_client::unsubscribe(std::string topic) {
    try {
        auto tok = this->client->unsubscribe(topic);
        tok->wait();
        return tok->get_return_code();
    } catch (const mqtt::exception &exc) {
        emit this->sendStatusText(exc.what());
        return 1;
    }
}

void mqtt_client::setCurrentTopic(std::string topic) {
    this->currentTopic = topic;
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