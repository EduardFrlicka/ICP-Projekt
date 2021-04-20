#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#include "ui_image_form.h"
#include <QWidget>

class ImageForm : public QWidget, private Ui::ImageForm {
    Q_OBJECT

  public:
    explicit ImageForm(QWidget *parent = nullptr);
    void SetImage(QPixmap *image);
};
#endif
