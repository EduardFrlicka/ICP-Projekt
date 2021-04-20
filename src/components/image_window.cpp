#include "image_window.h"

ImageForm::ImageForm(QWidget *parent) : QWidget(parent) { setupUi(this); }

void ImageForm::SetImage(QPixmap *image) { this->label->setPixmap(*image); }