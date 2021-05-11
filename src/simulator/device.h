#include "mqtt/async_client.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class basicDevice {
  public:
    basicDevice(int repeat, string address, string topic);
    vector<string> messages{};
    mqtt::async_client *client;
    int repeat = 1;
    string address;
    string topic;
    void send_message();
    void callback(mqtt::const_message_ptr msg);
};

class deviceThermostat : public basicDevice {
  public:
    int status;
    int target;
    deviceThermostat(int repeat, string address, string topic, int status) : basicDevice(repeat, address, topic) {
        this->status = status;
        this->target = status;
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    void send_message();
    void update();
    void callback(mqtt::const_message_ptr msg);
};

class deviceLights : public basicDevice {
  public:
    int status;
    deviceLights(int repeat, string address, string topic, int status) : basicDevice(repeat, address, topic) {
        this->status = status;
        this->messages = {"off", "on"};
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    void callback(mqtt::const_message_ptr msg);
    void send_message();
};
class deviceCofeeMachine : public basicDevice {
  public:
    int status;
    deviceCofeeMachine(int repeat, string address, string topic) : basicDevice(repeat, address, topic) {
        this->status = 0;
        this->messages = {"Ready", "Preparing to make cofee", "Making cofee", "Cofee ready", "Ready"};
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    void callback(mqtt::const_message_ptr msg);
    void update();
    void send_message();
};

enum deviceType {
    eBasicDevice,
    eThermostat,
    eCofeeMachine,
    eLights,
};

union device_u {
    basicDevice *device;
    deviceThermostat *thermostat;
    deviceLights *lights;
    deviceCofeeMachine *coffeeMachine;
};

class device {
  public:
    deviceType type;
    device_u ptr;

    device(basicDevice *ptr) {
        this->ptr.device = ptr;
        this->type = eBasicDevice;
    }
    device(deviceThermostat *ptr) {
        this->ptr.thermostat = ptr;
        this->type = eThermostat;
    }
    device(deviceLights *ptr) {
        this->ptr.lights = ptr;
        this->type = eLights;
    }
    device(deviceCofeeMachine *ptr) {
        this->ptr.coffeeMachine = ptr;
        this->type = eCofeeMachine;
    }

    void send_message() {
        // cout << "fatk dement" << this->type << endl;
        switch (this->type) {
        case eBasicDevice:
            this->ptr.device->send_message();
            break;
        case eCofeeMachine:
            this->ptr.coffeeMachine->send_message();
            break;
        case eThermostat:
            this->ptr.thermostat->send_message();
            break;
        case eLights:
            this->ptr.lights->send_message();
            break;

        default:
            std::cout << "Dement nemas naimplementovany send message\n";
            break;
        }
        // ptr[(int)type]->send_message();
    }
    // void get_device(){
    //     return ptr[(int)type];
    // }
};
