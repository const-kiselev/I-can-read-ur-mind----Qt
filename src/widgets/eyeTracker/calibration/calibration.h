#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <additional_header.h>

class Calibration : public QWidget
{
    Q_OBJECT
public:
    explicit Calibration(QWidget *parent = nullptr);
    int openCalibrationWidget();
    void moveTo(double, double);
signals:

public slots:
    int xDisp, yDisp;
private:
    void printPoint(int, int);

};

#endif // CALIBRATION_H
