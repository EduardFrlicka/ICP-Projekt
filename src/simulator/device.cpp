/**
 * @file device.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Source file for devices. Declaring classes functions for simulator
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */


#include "device.h"

string random_str(int len = 16) {
    string res = "";
    for (int i = 0; i < len; i++)
        res += "ABCDEFGHIJKLMNOPQRSTUVXYZabcdefghijklmnopqrstuvwxyz0123456789_"[rand() % 62];
    return res;
}

void basicDevice::send_message() {
    if (this->messages.size() == 0)
        return;
    int i = rand() % this->messages.size();
    try {
        client->publish(this->topic, this->messages[i]);
    } catch (const mqtt::exception &exc) {
    }
}

basicDevice::basicDevice(int repeat, string address, string topic) {
    this->address = address;
    this->repeat = repeat;
    this->topic = topic;

    client = new mqtt::async_client(this->address, random_str(), mqtt::create_options(MQTTVERSION_5));

    auto connOpts = mqtt::connect_options_builder().keep_alive_interval(std::chrono::seconds(3600)).mqtt_version(MQTTVERSION_5).clean_start(true).connect_timeout(std::chrono::seconds(5)).finalize();

    try {
        auto tok = client->connect(connOpts);
        tok->wait();
    } catch (const mqtt::exception &exc) {
        std::cout << "Error with connection to server: " << address << " | Error: " << exc.what() << std::endl;
    }

    auto subOpts = mqtt::subscribe_options(true);
    this->client->subscribe(topic, 0, subOpts)->wait();

    client->set_connection_lost_handler([this](const std::string &) { std::cout << "\n***Connection lost: " << this->address << " | Topic: " << this->topic << "***\n" << std::endl; });
}

void deviceThermostat::callback(mqtt::const_message_ptr msg) {
    this->target = stoi(msg->to_string());
    try {
        client->publish(this->topic, to_string(this->status));
    } catch (const mqtt::exception &exc) {
    }
}

void deviceThermostat::update() {
    this->status += ((this->status) < (this->target)) - ((this->status) > (this->target));
}

void deviceThermostat::send_message() {
    update();
    try {
        client->publish(this->topic, to_string(this->status));
    } catch (const mqtt::exception &exc) {
    }
}

void deviceLights::callback(mqtt::const_message_ptr msg) {
    std::cout << "Došla sprava: " << msg->to_string() << std::endl;
    this->status = "on" == msg->to_string();
    try {
        client->publish(this->topic, "turning lights " + this->messages[this->status]);
    } catch (const mqtt::exception &exc) {
    }
}

void deviceLights::send_message() {
    try {
        client->publish(this->topic, this->messages[this->status]);
    } catch (const mqtt::exception &exc) {
    }
}

void deviceCofeeMachine::update() {
    this->status = this->status + ((this->status) > 0);
    this->status %= this->messages.size();
}

void deviceCofeeMachine::callback(mqtt::const_message_ptr msg) {
    std::cout << "Došla sprava: " << msg->to_string() << std::endl;
    if (this->status) {
        try {
            client->publish(this->topic, "Busy");
        } catch (const mqtt::exception &exc) {
        }
        return;
    }
    this->status = 1;
}

void deviceCofeeMachine::send_message() {
    try {
        client->publish(this->topic, this->messages[this->status]);
    } catch (const mqtt::exception &exc) {
    }
    update();
}