#include "message_window.h"

MessageForm::MessageForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

void MessageForm::SetText(QString text) {
    this->textBrowser->setText(text);
}