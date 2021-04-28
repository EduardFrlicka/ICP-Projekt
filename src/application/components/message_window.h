/**
 * @file message_window.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Hedar file for Image form. Used to show messages in new form.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include "ui_message.h"
#include <QWidget>

class MessageForm : public QWidget, private Ui::MessageForm {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Message Form object
     *
     * @param parent
     */
    explicit MessageForm(QWidget *parent = nullptr);
    /**
     * @brief Assign text to textBrowser object.
     *
     * @param text data payload
     */
    void SetText(QString text);
};
#endif
