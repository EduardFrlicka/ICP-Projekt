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