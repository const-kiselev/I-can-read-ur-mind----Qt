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
    connect(this, SIGNAL(sendSignal(QString)), (MainWindow*)parent,
            SLOT(signalsHandler(QString)), Qt::QueuedConnection);
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
    connect(pcmd1, &QPushButton::clicked, this,[=](){ openCalibrationWidget();
        QTimer::singleShot(1000, this, [=](){emit sendSignal(MENU_START_CALIBRATION);});
                                                    });
}

void Calibration::testMoveWindow()
{
    QWidget *testMoveWin = new QWidget;
    QPushButton *pcmd1 = new QPushButton("Начать калибровку");
    QPushButton *pcmd2 = new QPushButton("Loading page...");
    QLabel *plblX = new QLabel("X value:");
    QLabel *plblY = new QLabel("Y value:");
    QLabel *plblTime = new QLabel("Time:");
    QLineEdit *ptxtEditX = new QLineEdit;
    QLineEdit *ptxtEditY = new QLineEdit;
    QLineEdit *ptxtEditTime = new QLineEdit;
    QVBoxLayout *pvbxLayout = new QVBoxLayout();
    pvbxLayout->addWidget(plblX);
    pvbxLayout->addWidget(ptxtEditX);
    pvbxLayout->addWidget(plblY);
    pvbxLayout->addWidget(ptxtEditY);
    pvbxLayout->addWidget(plblTime);
    pvbxLayout->addWidget(ptxtEditTime);
    pvbxLayout->addWidget(pcmd1);
    pvbxLayout->addWidget(pcmd2);
    testMoveWin->setLayout(pvbxLayout);
    testMoveWin->show();
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){this->moveTo(ptxtEditX->text().toDouble(), ptxtEditY->text().toDouble(), ptxtEditTime->text().toInt());});
    connect(pcmd2, &QPushButton::clicked, this, &Calibration::loading);
}

void Calibration::loading()
{
    QWidget* tmpWid = new QWidget;
    QMovie *movie = new QMovie(":/img/spinner");
    QVBoxLayout *pvbx = new QVBoxLayout;
    QLabel *processLabel = new QLabel;
    processLabel->setMovie(movie);
     movie->start();
     pvbx->addWidget(processLabel);
     pvbx->addWidget(new QLabel("<CENTER>Обработка данных...</CENTER>"));
     pvbx->setAlignment(Qt::AlignCenter);
     tmpWid->setLayout(pvbx);
    stLayout->addWidget(tmpWid);
    calibWidgetOpen = false;
    stLayout->setCurrentWidget(tmpWid);

}

void Calibration::openCalibrationWidget()
{
    QWidget *tmpOne = new QWidget;
    stLayout->addWidget(tmpOne);
    stLayout->setCurrentWidget(tmpOne);
    calibWidgetOpen = true;
    setPos(QPointF(-circleRect.width(), height()));
    if(NO_ET)
    {
        testMoveWindow();
        return;
    }
    update();

}


void Calibration::moveTo(double inX, double inY, int msTime)
{
    qDebug() << "I'm in moveTo()";
    propertyAnimation = new QPropertyAnimation(this, "pos");
    propertyAnimation->setDuration(msTime);
    propertyAnimation->setStartValue(QPointF(circleRect.x(), circleRect.y()));
    propertyAnimation->setEndValue(QPointF(width()*inX, height()*inY));
    propertyAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    propertyAnimation->start();
}

void Calibration::paintEvent(QPaintEvent *)
{
    qDebug() << "Try to print";
    if (!calibWidgetOpen) return;
    circleStandartPainter->begin(this); // todo: change the logic of circleRect var!!!!
    circleStandartPainter->drawPixmap(circleRect.x(), circleRect.y(), 30, 30,*circlePixmap);
    circleStandartPainter->end();
}
