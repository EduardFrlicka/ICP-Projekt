#ifndef WINDOW_H
#define WINDOW_H

#include "../mqtt.h"
#include "QFileDialog"
#include "image_window.h"
#include "server_dialog.h"
#include "ui_window.h"
#include <QWidget>

class Mqtt_explorer : public QWidget, private Ui::Mqtt_explorer {
    Q_OBJECT

  public:
    explicit Mqtt_explorer(QWidget *parent = nullptr);
    QString serverAdress;
    QString clientName;
    mqtt_client client;

  public slots:
    void addMessage(QString msg);
  private slots:
    void on_send_btn_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_attachFile_btn_clicked();
};

#endif
