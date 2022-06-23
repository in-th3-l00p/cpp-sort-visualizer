#include "visualizer.h"
#include <QSizePolicy>
#include <QPainter>
#include <QLabel>
#include <cmath>

Visualizer::Visualizer(QWidget *parent, int elements)
    : QWidget(parent)
{
    // ui settings
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(Properties::minimumSize);

    // setting up the array
    arr = std::vector<int>(elements);
    for (int i = 0; i < arr.size(); i++)
        arr[i] = i + 1;
}

void Visualizer::paintEvent(QPaintEvent *event) {
    QPainter painter{this};
    int canvasWidth = event->rect().width();
    int canvasHeight = event->rect().height();

    // drawing the background
    painter.setBrush(Qt::black);
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, canvasWidth - 1, canvasHeight - 1);

    // drawing the array's elements
    int elementWidth = static_cast<int>(
        ceil(static_cast<double>(canvasWidth) / arr.size())
    );
    int elementUnitHeight = static_cast<int>(
        ceil(static_cast<double>(canvasHeight) / arr.size())
    );

    painter.setBrush(Qt::cyan);
    painter.setPen(Qt::darkBlue);
    for (int i = 0; i < arr.size(); i++) {
        int elementHeight = elementUnitHeight * arr[i];
        painter.drawRect(
            i * elementWidth,
            canvasHeight - elementHeight,
            elementWidth,
            elementHeight
        );
    }
}







