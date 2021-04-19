#include "window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Mqtt_explorer win;
    win.show();
    return app.exec();
}
