#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <QPainter>
#include "../additional_header.h"

#define NUM_OF_CALIBRATION_FRAMES 100

class Calibration : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(QPoint circleFrame READ pos WRITE setCircleFrame)
public:
    explicit Calibration(QWidget *parent = nullptr);

    void moveTo(double, double, int);
    void setPos(QPointF inPointF)
    {
        circleRect.setX(inPointF.x());
        circleRect.setY(inPointF.y());
        this->repaint();
    }
    void setCircleFrame(QPoint inPoint)
    {
        //spriteFrame.setX(inPoint.x());
        spriteFrame.setY(inPoint.y()*50);
        spriteFrame.setWidth(50);
        spriteFrame.setHeight(50);
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
    QPropertyAnimation *propertyAnimation,
                       *circlePropertyAnimation;
    QRect circleRect;
    QRect spriteFrame;

    void makeSprite();
    void viewStartButtonWidget();
    void openCalibrationWidget();
};

#endif // CALIBRATION_H
