#include "gaze-point-test.h"

GazePointTest::GazePointTest(QWidget *parent) : QWidget(parent), circlePixmap(nullptr),
    propertyAnimation(nullptr), wayTime(0)
{
    circleTarget = circleRect = QRect(0,0,10,10);
    circleStandartPainter = new QPainter;
    drawPoint();
}

void GazePointTest::moveTo(double inX, double inY)
{
    propertyAnimation = new QPropertyAnimation(this, "pos");
    propertyAnimation->setDuration(wayTime);
    propertyAnimation->setStartValue(QPointF(circleTarget.x(), circleTarget.y()));
    propertyAnimation->setEndValue(QPointF(width()*inX, height()*inY));
    propertyAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    propertyAnimation->start();
    qDebug() << "GazePointTest::moveTo x = " << width()*inX << " y = " << height()*inY;
}

void GazePointTest::paintEvent(QPaintEvent *)
{
    circleStandartPainter->begin(this); // todo: change the logic of circleRect var!!!!
    circleStandartPainter->drawPixmap(circleTarget,*circlePixmap, circleRect);
    circleStandartPainter->end();
}

void GazePointTest::closeEvent(QCloseEvent *)
{

}

void GazePointTest::drawPoint()
{
    if(circlePixmap)
        return;

    circlePixmap = new QPixmap(circleRect.width(), circleRect.height());

    circlePixmap->setMask(circlePixmap->createHeuristicMask()); // set the alpha!!!
    circleStandartPainter->begin(circlePixmap);
    circleStandartPainter->setRenderHint(QPainter::Antialiasing, true);
    circleStandartPainter->setPen(QPen(Qt::NoPen));
    circleStandartPainter->setBrush(QBrush(QColor(236,11,67,255), Qt::SolidPattern));
    circleStandartPainter->drawEllipse(circleRect);
    circleStandartPainter->end();
}
