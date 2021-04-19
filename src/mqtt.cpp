#include "mqtt.h"

mqtt_client::mqtt_client(){};
mqtt_client::~mqtt_client(){};

void mqtt_client::create(std::string ADDRESS, std::string CLIENTID) {

    client = new mqtt::async_client(ADDRESS, CLIENTID);

    auto connOpts = mqtt::connect_options_builder().clean_session().finalize();
    connOpts.set_connect_timeout(1);

    try {
        cout << "\nConnecting..." << endl;
        mqtt::token_ptr conntok = client->connect(connOpts);
        cout << "Waiting for the connection..." << endl;
        conntok->wait();
        cout << "  ...OK" << endl;

    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }

    client->set_connection_lost_handler([](const std::string &) {
        std::cout << "*** Connection Lost  ***" << std::endl;
        exit(2);
    });

    // Set the callback for incoming messages

    client->set_message_callback([this](mqtt::const_message_ptr msg) {
        std::cout << msg->get_payload_str() << std::endl;
        emit this->getMessage(QString::fromStdString(msg->get_payload_str()));
    });
    // We publish and subscribe to one topic,
    // so a 'topic' object is helpful.

    const int QOS = 1;
    topic = new mqtt::topic{*client, "chat/1", QOS};

    // Start the connection.

    try {

        std::cout << "Ok\nJoining the group..." << std::flush;
        const bool NO_LOCAL = true;

        auto subOpts = mqtt::subscribe_options(NO_LOCAL);
        topic->subscribe(subOpts)->wait();
        std::cout << "Ok" << std::endl;
    } catch (const mqtt::exception &exc) {
        std::cerr << "\nERROR: Unable to connect. " << exc.what() << std::endl;
    }
}

void mqtt_client::disconnect() {

    try {
        cout << "\nDisconnecting..." << endl;
        client->disconnect()->wait();
        cout << "  ...OK" << endl;
    } catch (const mqtt::exception &exc) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", exc.what());
        messageBox.setFixedSize(500, 200);
        exit(1);
    }
}

void mqtt_client::sendMsg(std::string msg) { topic->publish(msg); }
