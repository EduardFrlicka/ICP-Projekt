#include "main.h"

int main(int argc, char *argv[]) {
    ifstream file(CONF);
    string str;
    vector<device *> devices;
    regex device_regex(".DEVICE ([1-9][0-9]*) (.*) (.*)");
    regex image_regex(".IMAGE (.*)");
    smatch match;
    device *current = NULL;

    while (getline(file, str)) {

        if (regex_search(str, match, device_regex)) {
            cout << "DEVICE: " << match[1] << " " << match[2] << " address: " << match[3].str() << '\n';
            current = new device(match[2].str(), stoi(match[1].str().c_str()), match[3].str());
            devices.push_back(current);
            continue;
        }

        if (regex_search(str, match, image_regex)) {
            cout << "IMAGE: " << match[1] << '\n';
            current->messages.push_back(slurp(match[1].str()));
            continue;
        }

        if (!current) {
            cerr << "bad config\n";
            return 1;
        }

        current->messages.push_back(str);
    }
    cout << "=====================================\n";
    int time = 1;
    while (time++) {
        sleep(1); // TODO change for chrono sleep
        for (auto device : devices) {
            if (0 == time % device->repeat)
                device->send_message();
        }
    }
}