/**
 * @file window.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Main window of MQTT simple explorer
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef WINDOW_H
#define WINDOW_H

#include "../mqtt.h"
#include "QFileDialog"
#include "camera_widget.h"
#include "coffee_widget.h"
#include "image_window.h"
#include "light_widget.h"
#include "message_window.h"
#include "server_dialog.h"
#include "thermometer_widget.h"
#include "thermostat_widget.h"
#include "ui_window.h"
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMap>
#include <QSettings>
#include <QWidget>
#include <filesystem>
#include <fstream>
#include <iostream>

#define MAX_MESSAGE_LINE_LENGTH 50

class window : public QMainWindow, private Ui::window {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new window object.
     * Main window inicialization. Creates server dialog and recieve server adress for mqtt conection.
     * Creates mqtt client object for mqtt comunication and connects signals trought application.
     * Calls loadConfig() function
     *
     * @param parent
     */
    explicit window(int maxMessages, QWidget *parent = nullptr);

    /**
     * @brief Triggers when apllication is closed.
     * Clears memory.
     *
     * @param event
     */
    void closeEvent(QCloseEvent *event);

    mqtt_client client;

    /**
     * @brief Map for messages history
     *
     */
    QMap<QString, QList<QListWidgetItem *>> messages;

    int maxMessageHistory;

  public slots:
    /**
     * @brief Arriving messages handling.
     *
     * Handles messages. Creats items, style them and appends to listWidgets.
     * Appending messages to history of messages and creates a record in treeWidget of last sended message.
     *
     * @param msg data payload
     * @param topicName
     * @param my_message indicates if message is sended by my client
     */
    void addMessage(QByteArray msg, QString topicName, int my_message = 0);

    /**
     * @brief Set the Status Bar Text
     * Sets status bar text for 1s
     *
     * @param msg message to be displayed
     */
    void setStatusBarText(QString msg);

  private slots:
    /**
     * @brief Loads app config
     * Load options from config file.
     * Loads widgets and add them to application window.
     */
    void loadConfig();

    // ede okomentuj
    void addNewTopic(QString topicName);
    // ede okomentuj
    QTreeWidgetItem *findTopic(QString topicName);
    // ede okomentuj
    QTreeWidgetItem *findTopicRecursive(QString topicName, int *i);

    /**
     * @brief Handle send btn clicked event
     * Takes input from textEdit object and send them to current topic.
     */
    void on_send_btn_clicked();

    /**
     * @brief Handle listWidget item clicked event
     * Load data stored in custom item variable.
     * Try parse this data to image. If it is image show image in new form.
     * If parse failed show as plain text in new form.
     *
     * @param item choosen item
     */
    void on_listWidget_itemClicked(QListWidgetItem *item);

    /**
     * @brief Handle listWidget_all item clicked event
     * Call handle function on_listWidget_itemClicked(QListWidgetItem *item)
     *
     * @param item choosen item
     */
    void on_listWidget_all_itemClicked(QListWidgetItem *item);

    /**
     * @brief Handle treeWidget item clicked event
     * Gets topic from clicked item.
     * If topic is subscribed, enable panel and sets current topic variable
     * If topic is not subscribed, disable panel and sets current topic variable to empty
     *
     * @param item choosen item
     * @param column
     */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    /**
     * @brief Handle actionSnapshot triggered event
     * Create folder tree structure based on current treeWidget setup to choosen dicetory.
     * For each subscribed topic creates payload.txt file thats stores messages.
     * If topic contains images store them too.
     *
     * @param checked unused
     */
    void on_actionSnapshot_triggered(bool checked);

    /**
     * @brief Handle subscribe btn click event
     * Try to subscribe new topic and print status message to statusbar.
     *
     */
    void on_subscribe_btn_clicked();

    /**
     * @brief Handle unsubscribe btn click event
     * Unsubscribe mqtt topic and delete node from treeWidget (only if list node).
     * Disable panel
     *
     */
    void on_unsubscribe_btn_clicked();

    /**
     * @brief Handle attachFile btn click event
     * Sends choosen image to current topic
     *
     */
    void on_attachFile_btn_clicked();

    /**
     * @brief Handle addWidget btn click event
     * Calls addWidget function to create new widget.
     * On sucess add new record of widget to config.
     */
    void on_addWidget_btn_clicked();

    /**
     * @brief Add new widget to main window
     *
     * @param name name of widget to be displayed
     * @param topic topic to connect
     * @param type type of widget
     * @param widgetID
     * @return 1 sucess
     * @return 0 failed
     */
    int addWidget(QString name, QString topic, int type, QString widgetID);
};

#endif
