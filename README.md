# ICP project - variant 1: MQTT explorer/dashboard

MQTT Explorer is a MQTT client that provides a structured overview of your MQTT topics.  
Dashboard allows you to create widgets for each device connected to a specific topic.  

The application met all the requirements of the assignment.

## Usage
App can be run by "make run" or by executing "app" binary in "bin" folder  
Simulator can be run by executing "sim" binary from "bin" folder  
```
App Options:
-h, --help                 Displays help.
-c, --config               Shows path to config file.
-m, --max-messages <count> Set message history limit.
```

## Technologies used

Qt version 5.12.8  
[MQTT paho for c++](https://github.com/eclipse/paho.mqtt.cpp) | Paho C++ Version 1.2.0 | Requires Paho C v1.3.8  
c++ 17

## Authors

Peter Zdravecký - xzdrav00  
Euard Frlička - xfrlic00
