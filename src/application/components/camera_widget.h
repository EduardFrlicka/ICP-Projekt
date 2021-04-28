/**
 * @file camera_widget.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for Camera Widget. Input handling and internal setup.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef CAMERA_WIDGET_H
#define CAMERA_WIDGET_H

#include "../mqtt.h"
#include "image_window.h"
#include "ui_camera_widget.h"
#include <QDateTime>
#include <QSettings>
#include <QWidget>

class CameraWidget : public QWidget, private Ui::CameraWidget {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Camera Widget object
     *
     * @param parent parent object
     * @param client active mqtt connection client
     * @param name name of device
     * @param topic topic to interact with. (mqtt topic)
     * @param widgetID
     */
    explicit CameraWidget(QWidget *parent, mqtt_client *client, QString name, QString topic, QString widgetID);
    QString name;
    QString topic;
    QString widgetID;
    QPixmap img;

  public slots:

    /**
     * @brief New message arrived handling.
     *
     * Checks new message. If subscribed topic is same as widget topic message is accepted.
     * Message check if it's image or unknown data.
     * Sets status label output and sets class variable 'img'.
     *
     * @param msg message payload
     * @param topicName
     */
    void addMessage(QByteArray msg, QString topicName);

    /**
     * @brief Shows last arrived image.
     *
     * Creates new image form and sets image from class variable 'img' .
     * Then show image form.
     *
     */
    void on_show_btn_clicked();

    /**
     * @brief Delete widget from layout and delete from config file.
     *
     */
    void on_delete_btn_clicked();
};
#endif