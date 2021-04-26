#ifndef MESSAGE_H
#define MESSAGE_H

#include "ui_message.h"
#include <QWidget>

#define DEFAULT_ADRESS "tcp://broker.emqx.io:1883"

class MessageForm : public QWidget, private Ui::MessageForm {
    Q_OBJECT

  public:
    explicit MessageForm(QWidget *parent = nullptr);
    void SetText(QString text);
};
#endif
