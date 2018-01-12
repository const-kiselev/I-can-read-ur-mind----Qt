#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <QPainter>
#include "src/additional_header.h"

#define NUM_OF_CALIBRATION_FRAMES 100

class Calibration : public QWidget
{
    Q_OBJECT
public:
    explicit Calibration(QWidget *parent = nullptr);
    int openCalibrationWidget();
    void moveTo(double, double, int);
signals:

public slots:
private:
    int xDisp, yDisp;
    void printPoint(int, int);
    QPainter *calibPainter;


};

#endif // CALIBRATION_H
