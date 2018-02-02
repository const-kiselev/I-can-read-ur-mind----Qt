#ifndef GAZEPOINTTEST_H
#define GAZEPOINTTEST_H

#include <QWidget>
#include <QPainter>
#include "src/additional_header.h"

class GazePointTest : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    GazePointTest(QWidget *parent = nullptr);
    void moveTo(double inX, double inY); // TODO: реализовать методc
    void setPos(QPointF inPointF)
    {
        circleTarget.setX(inPointF.x());
        circleTarget.setY(inPointF.y());
        this->repaint();
    }

protected:
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);
private:
    void drawPoint();

    QPainter *circleStandartPainter;
    QPixmap *circlePixmap;
    int wayTime;
    QPropertyAnimation *propertyAnimation; // movement animation property
    QRect circleRect, circleTarget;
};

#endif // GAZEPOINTTEST_H
