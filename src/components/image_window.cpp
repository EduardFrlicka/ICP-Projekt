#include "image_window.h"

ImageForm::ImageForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

void ImageForm::SetImage(QPixmap *image) {
    QGraphicsScene *scene = new QGraphicsScene();
    auto item = new QGraphicsPixmapItem(*image);

    scene->addItem(item);
    this->graphicsView->setScene(scene);
}