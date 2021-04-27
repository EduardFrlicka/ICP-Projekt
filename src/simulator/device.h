#include "mqtt/async_client.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class device {
  public:
    device(string name, int repeat, string topic);
    vector<string> messages{};
    mqtt::async_client *client;
    int repeat = 1;
    string name;
    string topic;
    void send_message();
};