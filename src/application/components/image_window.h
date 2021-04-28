/**
 * @file image_window.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Hedar file for Image form. Used to show images in new form.
 * @version 0.1
 * @date 2021-04-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#include "ui_image_form.h"
#include <QGraphicsPixmapItem>
#include <QWidget>

class ImageForm : public QWidget, private Ui::ImageForm {
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Image Form object
     *
     * @param parent
     */
    explicit ImageForm(QWidget *parent = nullptr);

    /**
     * @brief Assign image data to graphicsView object in form
     *
     * @param image data payload
     */
    void SetImage(QPixmap *image);
};
#endif
