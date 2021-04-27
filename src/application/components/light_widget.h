#ifndef LIGHT_WIDGET_H
#define LIGHT_WIDGET_H

#include "ui_light_widget.h"
#include <QWidget>

class LightWidget : public QWidget, private Ui::LightWidget {
    Q_OBJECT

  public:
    explicit LightWidget(QWidget *parent = nullptr);
};
#endif
