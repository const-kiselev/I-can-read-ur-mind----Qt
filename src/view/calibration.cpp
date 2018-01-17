#include "calibration.h"

Calibration::Calibration(QWidget *parent) : QWidget(parent)
{
    circlePixmap = nullptr;
    calibWidgetOpen = false;
    wayTime = 800;

    stLayout = new QStackedLayout;
    QVBoxLayout *vbxLayout = new QVBoxLayout;
    vbxLayout->addLayout(stLayout);
    setLayout(vbxLayout);
    viewStartButtonWidget();
    // draw the pixmap of circle
    circleStandartPainter = new QPainter;
    makeSprite();
}

void Calibration::makeSprite()
{
    if(circlePixmap){
        delete circlePixmap;
        circlePixmap = nullptr;
    }
    else
        circlePixmap = new QPixmap(30, 30);
    circlePixmap->setMask(circlePixmap->createHeuristicMask()); // set the alpha!!!
    circleStandartPainter->begin(circlePixmap);
    circleStandartPainter->setRenderHint(QPainter::Antialiasing, true);
    circleStandartPainter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    circleStandartPainter->setPen(QPen(Qt::NoPen));
    circleStandartPainter->drawEllipse(0,0,30, 30);
    circleStandartPainter->end();
    circleRect = QRect(0,0,30,30);
}

void Calibration::viewStartButtonWidget()
{
    QPushButton *pcmd1 = new QPushButton("Начать калибровку");
    QVBoxLayout *pvbxLayout = new QVBoxLayout();
    pvbxLayout->addWidget(pcmd1);
    stLayout->addWidget(pcmd1);
    stLayout->setCurrentWidget(pcmd1);
    connect(pcmd1, &QPushButton::clicked, this,[=](){ openCalibrationWidget();});
}

void Calibration::openCalibrationWidget()
{
    QWidget *tmpOne = new QWidget;
    stLayout->addWidget(tmpOne);
    stLayout->setCurrentWidget(tmpOne);
    calibWidgetOpen = true;
    setPos(QPointF(-circleRect.width(), height()));
    update();
    emit sendSignal(MENU_START_CALIBRATION);
    qDebug() << "after sendSignal(MENU_START_CALIBRATION);";
}


void Calibration::moveTo(double inX, double inY, int msTime)
{
    propertyAnimation = new QPropertyAnimation(this, "pos");
    propertyAnimation->setDuration(msTime);
    propertyAnimation->setStartValue(QPointF(circleRect.x(), circleRect.y()));
    propertyAnimation->setEndValue(QPointF(width()*inX, height()*inY));
    propertyAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    propertyAnimation->start();
}

void Calibration::paintEvent(QPaintEvent *)
{
    if (!calibWidgetOpen) return;
    circleStandartPainter->begin(this); // todo: change the logic of circleRect var!!!!
    circleStandartPainter->drawPixmap(circleRect.x(), circleRect.y(), 30, 30,*circlePixmap);
    circleStandartPainter->end();
}
