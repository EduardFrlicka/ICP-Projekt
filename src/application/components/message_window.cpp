/**
 * @file message_window.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "message_window.h"

MessageForm::MessageForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

void MessageForm::SetText(QString text) {
    this->textBrowser->setText(text);
}