#include "image_window.h"

ImageForm::ImageForm(QWidget *parent) : QWidget(parent) { setupUi(this); }

void ImageForm::SetImage(QPixmap *picture) { this->label->setPixmap(*picture); }