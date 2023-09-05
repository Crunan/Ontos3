#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H

#include <QWidget>

class StageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StageWidget(QWidget* parent = nullptr);

    void setStageBounds(double minX, double maxX, double minY, double maxY);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double minX;
    double maxX;
    double minY;
    double maxY;
};

#endif // STAGEWIDGET_H
