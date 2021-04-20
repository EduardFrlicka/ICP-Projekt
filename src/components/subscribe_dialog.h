#ifndef SUBSCRIBE_DIALOG_H
#define SUBSCRIBE_DIALOG_H

#include "ui_subscribe_dialog.h"
#include <QDialog>

class SubscribeDialog : public QDialog, private Ui::SubscribeDialog {
    Q_OBJECT

  public:
    SubscribeDialog(QWidget *parent = nullptr);
    QString getTopicName();
};
#endif
