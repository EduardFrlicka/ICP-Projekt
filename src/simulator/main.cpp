/**
 * @file main.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Source file for simulator
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "main.h"

int main(int argc, char *argv[]) {
    // randomizing
    srand(time(NULL));
    
    // opening config file
    ifstream file(CONF);
    
    // some variables
    string str;
    vector<device *> devices;
    device *current = NULL;
    
    // regexes for config parsing
    regex device_regex("\\.DEVICE ([1-9][0-9]*) (.*) (.*)");
    regex thermostat_regex(".THERMOSTAT ([1-9][0-9]*) (.*) (.*) (-?[0-9]+)");
    regex lights_regex(".LIGHTS ([1-9][0-9]*) (.*) (.*)");
    regex cofeeMachine_regex(".COFFEE ([1-9][0-9]*) (.*) (.*)");
    regex image_regex("\\.IMAGE (.*)");
    smatch match;
    

    while (getline(file, str)) {
        
        // basic device - no message callback, just sending random messages
        if (regex_search(str, match, device_regex)) {
            cout << "DEVICE: " << match[1] << " address: " << match[2] << " topic: " << match[3].str() << '\n';
            current = new device(new basicDevice(stoi(match[1].str().c_str()), match[2].str(), match[3].str()));
            devices.push_back(current);
            continue;
        }

        // thermostat - sending current temperature, able to set target temperature
        if(regex_search(str, match, thermostat_regex)){
            cout << "THERMOSTAT: " << match[1] << " address: " << match[2] << " topic: " << match[3].str() << " start status: "<<match[4].str() << '\n';
            current = new device(new deviceThermostat(stoi(match[1].str()), match[2].str(), match[3].str(),stoi(match[4].str())));
            devices.push_back(current);
            continue;
        }
        
        // lights - user can turn them on or off
        if(regex_search(str, match, lights_regex)){
            cout << "LIGHTS: " << match[1] << " address: " << match[2] << " topic: " << match[3].str() << '\n';
            current = new device(new deviceLights(stoi(match[1].str()), match[2].str(), match[3].str(),0));
            devices.push_back(current);
            continue;
        }
        
        // coffee machine - after "make" message, will make coffee
        if(regex_search(str, match, cofeeMachine_regex)){
            cout << "COFFEE: " << match[1] << " address: " << match[2] << " topic: " << match[3].str() << '\n';
            current = new device(new deviceCofeeMachine(stoi(match[1].str()), match[2].str(), match[3].str()));
            devices.push_back(current);
            continue;
        }

        // Message parsing:
        if (!current) {
            cerr << "bad config\n";
            return 1;
        }

        // Image
        if (regex_search(str, match, image_regex)) {
            cout << "IMAGE: " << match[1] << '\n';
            current->ptr.device->messages.push_back(slurp(match[1].str()));
            continue;
        }
        
        // plain text message
        current->ptr.device->messages.push_back(str);
    }
    cout << "=====================================\n";

    
    // loop
    int time = 1;
    while (time++) {
        sleep(1); // TODO change for chrono sleep
        for (auto device : devices) {
            if (0 == time % device->ptr.device->repeat)
                device->send_message();
        }
    }
}