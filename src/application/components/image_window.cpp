/**
 * @file image_window.cpp
 * @authors Peter Zdravecký, Eduard Frlička
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
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