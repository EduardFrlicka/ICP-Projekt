#include "subscribe_dialog.h"

SubscribeDialog::SubscribeDialog(QWidget *parent) : QDialog(parent) { setupUi(this); }

QString SubscribeDialog::getTopicName() { return topicNameInput->text(); }