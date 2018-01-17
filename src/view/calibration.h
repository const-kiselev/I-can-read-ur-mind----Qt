#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <QPainter>
#include "src/additional_header.h"

#define NUM_OF_CALIBRATION_FRAMES 100

class Calibration : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit Calibration(QWidget *parent = nullptr);

    void moveTo(double, double, int);
    void setPos(QPointF inPointF)
    {
        circleRect.setX(inPointF.x());
        circleRect.setY(inPointF.y());
        this->repaint();
    }
    void loading();
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
protected:
    void paintEvent(QPaintEvent *);
private:
    QStackedLayout *stLayout;

    QPainter *circleStandartPainter;
    QPixmap *circlePixmap;
    QPoint circlePos;
    bool calibWidgetOpen;
    int wayTime;
    QPropertyAnimation *propertyAnimation;
    QRect circleRect;


    void makeSprite();
    void viewStartButtonWidget();
    void openCalibrationWidget();


};

#endif // CALIBRATION_H
