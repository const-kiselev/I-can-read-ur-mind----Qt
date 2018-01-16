#include "view.h"

View::View(QObject *parent) : QObject(parent)
{
    connect(_menu, SIGNAL(sendSignal(const QString&)),
            this, SLOT(handler(const QString&)));

    mainWindow = new MainWindow;
    mainWindow->init();
    mainWindow->show();
    // ADD resize!!!!
    // FOR macOS:
    //QTimer::singleShot(1000, &window, SLOT(showFullScreen()));
}

int View::init()
{
    _mainWindow = new MainWindow;
    _menu = new Menu;
}

void View::handler(const QString &inString)
{
    qDebug() << this->objectName() << " handler(): input = " << inString;

    if(inString == EYE_TRACKER_SUCESSFULL_INIT){
        _menu->addEyeTrackerActions();
        _mainWindow->addAndShowInViewStack(_menu);
    }
    else if(inString == CONTROLLER_ALL_GADGETS_SUCESSFULL_INITED){
        // todo!!!!
    }
    else if(inString == MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET){
        _eyeTrackerCalibrationWidget = new Calibration;
        _mainWindow->addAndShowInViewStack(_eyeTrackerCalibrationWidget);
    }
    else if(inString == VIEW_CALIBRATION_WIDGET_READY){
        controllerHandler(VIEW_CALIBRATION_WIDGET_READY);
    }

}
