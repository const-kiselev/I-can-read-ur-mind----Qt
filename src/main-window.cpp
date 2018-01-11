#include "main-window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedWidget = new QStackedWidget;
    startInitProcess();
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
}

int MainWindow::signalsHandler(const QString &inSignal)
{
    if(inSignal == EYE_TRACKER_SUCESSFULL_INIT){
        menuHandler(MENU_ADD_EYE_TRACKER_ACTIONS);
        breakInitProcess();
    }
    else if(inSignal == EYE_TRACKER_SUCESSFULL_CALIBRATION_START){

    }
    else if(inSignal == EYE_TRACKER_POINT_TO_CALIBRATE){

    }
    else if(inSignal == EYE_TRACKER_LEAVE_CALIBRATION_MODE){
        QWidget *tmpWidget = stackedWidget->currentWidget();
        stackedWidget->setCurrentWidget(pMenu);
        stackedWidget->removeWidget(tmpWidget);
        delete tmpWidget;
    }
    else if(inSignal == MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET){
        QWidget *tmpWidget = new Calibration(this);
        stackedWidget->addWidget(tmpWidget);
        stackedWidget->setCurrentWidget(tmpWidget);
    }
    else if(inSignal == CALIBRATION_WIDGET_REQUEST_START){
        ((Calibration*)stackedWidget->currentWidget())
                ->openCalibrationWidget();
        eyeTrakerHandler(EYE_TRACKER_START_CALIBRATION);
    }
    return 0;
}

int MainWindow::startInitProcess()
{
    //QWidget *pInitWidget = new QWidget(this);
    QLabel labelInit("Инициализация всех устройств");
    stackedWidget->addWidget(&labelInit);
    stackedWidget->setCurrentWidget(&labelInit);
    setCentralWidget(stackedWidget);

    pMenu = new Menu(this);
    connect(this, SIGNAL(menuHandler(QString)),
            pMenu, SLOT(signalsHandler(const QString&)));

    stackedWidget->addWidget(pMenu);

    emit eyeTrakerHandler("init");
    return 0;
}

int MainWindow::breakInitProcess()
{
    stackedWidget->setCurrentWidget(pMenu);
    return 0;
}
