#include "device.h"

void device::send_message() {
    int i = rand() % this->messages.size();
    try {
        client->publish(this->topic, this->messages[i]);
    } catch (const mqtt::exception &exc) {
    }
}

device::device(string name, int repeat, string topic) {
    this->name = name;
    this->repeat = repeat;
    this->topic = topic;

    client = new mqtt::async_client(this->name, "random", mqtt::create_options(MQTTVERSION_5));

    auto connOpts = mqtt::connect_options_builder().keep_alive_interval(std::chrono::seconds(3600)).mqtt_version(MQTTVERSION_5).clean_start(true).connect_timeout(std::chrono::seconds(5)).finalize();

    try {
        auto tok = client->connect(connOpts);
        tok->wait();
    } catch (const mqtt::exception &exc) {
        std::cout << "Error with connection to server: " << name << " | Error: " << exc.what() << std::endl;
    }

    auto subOpts = mqtt::subscribe_options(true);
    this->client->subscribe(topic, 0, subOpts)->wait();

    client->set_connection_lost_handler([this](const std::string &) { std::cout << "\n***Connection lost: " << this->name << " | Topic: " << this->topic << "***\n" << std::endl; });

    client->set_message_callback([this](mqtt::const_message_ptr msg) { std::cout << "Došla sprava: " << msg->to_string() << std::endl; });
}