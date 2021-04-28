/**
 * @file server_dialog.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for Server Dialog. Handle server adress input from user.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef SERVER_DIALOG_H
#define SERVER_DIALOG_H

#include "ui_server_dialog.h"
#include <QDialog>

/**
 * @brief Default adress for connection. For test purposes
 *
 */
#define DEFAULT_ADRESS "tcp://test.mosquitto.org:1883"

class ServerDialog : public QDialog, private Ui::ServerDialog {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Server Dialog object
     *
     * @param parent
     */
    ServerDialog(QWidget *parent = nullptr);

    /**
     * @brief Get the Server Adress
     *
     * @return text from server adress input
     */
    QString getServerAdress();
};
#endif
