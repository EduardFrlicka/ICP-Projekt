/**
 * @file main.cpp
 * @author Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "window.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("ICP-xzdrav00-xfrlic00");
    QCoreApplication::setApplicationName("Zadani-1");
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.setApplicationDescription("MQTT explorer/dashboard");
    QCommandLineOption maxMessagesOpiton(QStringList() << "m"
                                                       << "max-messages",
                                         QCoreApplication::translate("main", "Set message history limit"), QCoreApplication::translate("main", "count"));

    parser.addOption(maxMessagesOpiton);
    parser.process(app);

    QString maxMessagesValue = parser.value(maxMessagesOpiton);

    int maxMessages = 50; // defualt value
    if (!maxMessagesValue.isEmpty()) {
        bool ok;
        maxMessages = maxMessagesValue.toInt(&ok, 10);
        if (!ok) {
            std::cout << "option: -m, --message <count> | must be integer" << std::endl;
            return 1;
        }
    }

    window win(maxMessages);
    win.show();

    return app.exec();
}
