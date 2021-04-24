#ifndef WINDOW_H
#define WINDOW_H

#include "../mqtt.h"
#include "QFileDialog"
#include "image_window.h"
#include "server_dialog.h"
#include "subscribe_dialog.h"
#include "ui_window.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QWidget>
#include <fstream>
#include <iostream>

#define MAX_MESSAGE_HISTORY 10

// možno nejaké stuff s right clickom https://www.setnode.com/blog/right-click-context-menus-with-qt/#fnref:viewportclasses

class window : public QWidget, private Ui::window {
    Q_OBJECT

  public:
    explicit window(QWidget *parent = nullptr);
    mqtt_client client;
    QMap<QString, QList<QListWidgetItem *>> messages;
    void addNewTopic(QString topicName);

  public slots:
    void addMessage(QByteArray msg,QString topicName);

  private slots:
    QTreeWidgetItem *findTopic(QString topicName);
    QTreeWidgetItem *findTopicRecursive(QString topicName, int *i);
    void on_send_btn_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_subscribe_btn_clicked();
    void on_attachFile_btn_clicked();
};

#endif
