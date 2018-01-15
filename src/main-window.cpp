#include "main-window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedWidget = new QStackedWidget;
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
}

void MainWindow::init()
{
    startInitProcess();
}

int MainWindow::signalsHandler(const QString &inSignal)
{
    qDebug() << "MainWindow::signalsHandler recieved: " << inSignal << endl;
    if(inSignal == EYE_TRACKER_SUCESSFULL_INIT){
        menuHandler(MENU_ADD_EYE_TRACKER_ACTIONS);
        breakInitProcess();
    }
    else if(inSignal == EYE_TRACKER_SUCESSFULL_CALIBRATION_START){

    }
    else if(inSignal.mid(0,3) == EYE_TRACKER_POINT_TO_CALIBRATE){
        emit stdHandler(inSignal);
    }
    else if(inSignal == EYE_TRACKER_LEAVE_CALIBRATION_MODE){
        QWidget *tmpWidget = stackedWidget->currentWidget();
        stackedWidget->setCurrentWidget(pMenu);
        stackedWidget->removeWidget(tmpWidget);
        delete tmpWidget;
    }
    else if(inSignal == MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET){
        pMainCalib = new Calibration(this);
        stackedWidget->addWidget(pMainCalib);
        stackedWidget->setCurrentWidget(pMainCalib);
        connect(this, SIGNAL(stdHandler(QString)), (Calibration*)pMainCalib, SLOT(signalsHandler(QString)));
    }
    else if(inSignal==MENU_START_CALIBRATION){
        emit eyeTrakerHandler(MENU_START_CALIBRATION);
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

    emit eyeTrakerHandler(EYE_TRACKER_INIT);
    return 0;
}

int MainWindow::breakInitProcess()
{
    stackedWidget->setCurrentWidget(pMenu);
    return 0;
}
