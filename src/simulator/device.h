/**
 * @file device.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for devices. Declaring classes for simulator
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mqtt/async_client.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;
class basicDevice {
  public:
    /**
     * @brief Creates basicDevice object - connets to server, subscribes topic
     *
     * @param repeat "sleep time, between messages"
     *
     * @param address server address for mqtt
     *
     * @param topic topic for mqtt (will be subscribed)
     *
     */
    basicDevice(int repeat, string address, string topic);

    // messages to be sent
    vector<string> messages{};
    mqtt::async_client *client;
    int repeat = 1;

    // maybe not needed ?
    string address;
    string topic;
    /**
     * @brief sends random message from messages vector
     */
    void send_message();
};

class deviceThermostat : public basicDevice {
  public:
    int status;
    int target;
    /**
     * @brief Creates deviceThermostat object - connets to server, subscribes topic, sets starting status
     *
     * @param repeat "sleep time, between messages"
     *
     * @param address server address for mqtt
     *
     * @param topic topic for mqtt (will be subscribed)
     *
     * @param status starting "temperature", will be sat as status and as target (temperature will not change until message recieved)
     *
     */
    deviceThermostat(int repeat, string address, string topic, int status) : basicDevice(repeat, address, topic) {
        this->status = status;
        this->target = status;
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    /**
     * @brief updates and sends current status (integer representing current temperature)
     */
    void send_message();
    /**
     * @brief if target != status updates status (incremented or decremented by 1 towards target)
     */
    void update();
    /**
     * @brief sets target to int from message (function assuming that message will be integer)
     *
     * @param msg pointer to recieved message
     */
    void callback(mqtt::const_message_ptr msg);
};

class deviceLights : public basicDevice {
  public:
    int status;

    /**
     * @brief Creates deviceLights object - connets to server, subscribes topic
     *
     * @param repeat "sleep time, between messages"
     *
     * @param address server address for mqtt
     *
     * @param topic topic for mqtt (will be subscribed)
     *
     * @param status starting status of lights (0:off | 1:on)
     *
     */
    deviceLights(int repeat, string address, string topic, int status) : basicDevice(repeat, address, topic) {
        this->status = status;
        this->messages = {"off", "on"};
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    /**
     * @brief turns lights on if msg == "on" else turns them off
     *
     * @param msg pointer to recieved message
     */
    void callback(mqtt::const_message_ptr msg);
    /**
     * @brief sends message displaying current status ("on"|"off")
     */
    void send_message();
};
class deviceCofeeMachine : public basicDevice {
  public:
    int status;
    /**
     * @brief Creates basicDevice object - connets to server, subscribes topic
     *
     * @param repeat "sleep time, between messages"
     *
     * @param address server address for mqtt
     *
     * @param topic topic for mqtt (will be subscribed)
     *
     */
    deviceCofeeMachine(int repeat, string address, string topic) : basicDevice(repeat, address, topic) {
        this->status = 0;
        this->messages = {"Ready", "Preparing to make cofee", "Making cofee", "Cofee ready", "Ready"};
        client->set_message_callback([this](mqtt::const_message_ptr msg) { this->callback(msg); });
    }
    /**
     * @brief if machine is in "ready" state, starts making coffee
     *
     * @param msg pointer to recieved message
     */
    void callback(mqtt::const_message_ptr msg);
    /**
     * @brief if target != 0 (ready) updates status (incrementing | set to 0 if coffee making sequence ended)
     */
    void update();
    /**
     * @brief  updates and sends current status
     */
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
    /**
     * @brief Creates device object based on parameter given
     *
     * @param ptr could be pointer to any class declarred above
     */
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

    /**
     * @brief function calls right send_message() function, based on device type
     */
    void send_message() {
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
            break;
        }
    }
};
