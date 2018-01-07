#include "calibration.h"

Calibration::Calibration(QWidget *parent) : QWidget(parent)
{
    xDisp = yDisp = -1;
    QPushButton *pcmd1 = new QPushButton("Начать калибровку");
    QVBoxLayout *pvbxLayout = new QVBoxLayout();
    pvbxLayout->addWidget(pcmd1);
    this->setLayout(pvbxLayout);
}

int Calibration::openCalibrationWidget()
{

}

void Calibration::moveTo(double inX, double inY, int msTime)
{
    int xPrint, yPrint,
            xWidth = this->width(),
            yHeight = this->height();
    if((xDisp == yDisp) &&(xDisp == -1)){
        printPoint(inX*xWidth, inY*yHeight);
        xDisp = inX*xWidth;
        yDisp = inY*yHeight;
        return;
    }
    int destinyX = inX*xWidth,
            destinyY = inY*yHeight;
    double k,
            lenght = sqrt( (destinyX - xDisp)*(destinyX - xDisp) +
                             (destinyY - yDisp)*(destinyY - yDisp) );
    int sleepTime = msTime/lenght,
            resultY, resultX;

    for(int i=0; i <= lenght; i++){
        k = i/lenght;
        xPrint = xDisp + (inX - xDisp)*k;
        yPrint = yDisp + (inY - yDisp)*k;
        printPoint(xPrint, yPrint);
        QThread::msleep(sleepTime);
    }
    if( (xPrint!= destinyX) || (yPrint!=destinyY) )
        printPoint(destinyX, destinyY);
}
