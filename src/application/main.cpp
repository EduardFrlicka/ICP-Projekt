#include "window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("ICP-xzdrav00-xfrlic00");
    QCoreApplication::setApplicationName("Zadni-1");
    QApplication app(argc, argv);
    window win;
    win.show();
    return app.exec();
}
