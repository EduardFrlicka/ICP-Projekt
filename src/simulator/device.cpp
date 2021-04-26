#include "device.h"

void device::send_message() {
    int i = rand() % messages.size();
    cout << name + "/" + topic << ": " << messages[i] << endl;
}

device::device(string name, int repeat, string topic) {
    this->name = name;
    this->repeat = repeat;
    this->topic = topic;
}