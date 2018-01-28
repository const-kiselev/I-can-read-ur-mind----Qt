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
    layout()->setAlignment(stLayout, Qt::AlignCenter);
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
        circlePixmap = new QPixmap(50, 50*34);

    circlePixmap->setMask(circlePixmap->createHeuristicMask()); // set the alpha!!!
    circleStandartPainter->begin(circlePixmap);
    circleStandartPainter->setRenderHint(QPainter::Antialiasing, true);
    circleStandartPainter->setPen(QPen(Qt::NoPen));
    int a=0, b=1;
    for(int i=0; i<34; i++){
        circleStandartPainter->setBrush(QBrush(QColor(236,11,67,160), Qt::SolidPattern));
        circleStandartPainter->drawEllipse(QPoint(25, 25+50*i),25-a, 25-a);
        circleStandartPainter->setBrush(QBrush(QColor(25,70,186,255), Qt::SolidPattern));
        circleStandartPainter->drawEllipse(QPoint(25, 25+50*i), 2, 2);
        if(a==17)
            b=-b;
        a+=b;
    }
    circleStandartPainter->end();
    circleRect = QRect(0,0,50,50);
    spriteFrame = QRect(0,0,50,50);
    circlePropertyAnimation = new QPropertyAnimation(this, "circleFrame");
    circlePropertyAnimation->setDuration(1500);
    circlePropertyAnimation->setLoopCount(-1);
    circlePropertyAnimation->setStartValue(QPoint(spriteFrame.x(), spriteFrame.y()));
    circlePropertyAnimation->setEndValue(QPoint(0,33));
    circlePropertyAnimation->setEasingCurve(QEasingCurve::InBounce);

}

void Calibration::viewStartButtonWidget()
{
    QWidget *tmpWid = new QWidget;
    QLabel *instruction = new QLabel("Процесс калибровки необходим для корректной работы оборудования. Всё что от Вас требуется — следить за движущимся кружочком и при необходимости выполнять появляющиеся команды.");
    instruction->setWordWrap(true);
    instruction->setMaximumWidth(500);
    instruction->setFont(QFont( "Arial", 18, QFont::Normal));
    instruction->setAlignment(Qt::AlignCenter);
    QPushButton *pcmd1 = new QPushButton("Начать калибровку");
    pcmd1->setMinimumWidth(300);
    pcmd1->setMinimumHeight(100);
    QVBoxLayout *pvbxLayout = new QVBoxLayout();


    pvbxLayout->addWidget(instruction);
    pvbxLayout->setAlignment(instruction, Qt::AlignBottom|Qt::AlignHCenter);
    //pvbxLayout->setStretchFactor(instruction, 2);
    pvbxLayout->addWidget(pcmd1);
    tmpWid->setLayout(pvbxLayout);
    stLayout->addWidget(tmpWid);
    stLayout->setCurrentWidget(tmpWid);
    pvbxLayout->setAlignment(pcmd1, Qt::AlignTop|Qt::AlignHCenter);
    connect(pcmd1, &QPushButton::clicked, this,[=](){ openCalibrationWidget();});
}

void Calibration::openCalibrationWidget()
{
    QWidget *tmpOne = new QWidget;
    stLayout->addWidget(tmpOne);
    stLayout->setCurrentWidget(tmpOne);
    calibWidgetOpen = true;
    setPos(QPointF(-circleRect.width(), height()));
    circlePropertyAnimation->start();
    update();
    emit sendSignal(MENU_START_CALIBRATION);
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
    circleStandartPainter->drawPixmap(QRect(circleRect.x(), circleRect.y(), 50, 50),*circlePixmap, spriteFrame);
    circleStandartPainter->end();
}
