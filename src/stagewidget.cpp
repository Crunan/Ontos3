#include "include/stagewidget.h"

#include <QPainter>

StageWidget::StageWidget(QWidget* parent)
    : QWidget(parent), minX(0.0), maxX(0.0), minY(0.0), maxY(0.0)
{
}

void StageWidget::setStageBounds(double minX, double maxX, double minY, double maxY)
{
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
    update(); // Update the widget to trigger a repaint
}

void StageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(rect());
}
