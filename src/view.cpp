#include "view.h"

View::View(QObject *parent) : QObject(parent)
{
    // ADD resize!!!!
    // FOR macOS:
    //QTimer::singleShot(1000, &window, SLOT(showFullScreen()));
}

void View::init()
{
    _mainWindow = new MainWindow;
    connect(_mainWindow, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    _mainWindow->init();
    _mainWindow->show();
    _menu = new Menu;
    connect(_menu, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    emit controllerHandler(VIEW_INIT_COMRLETED);
    _mainWindow->showLoadingWidget();
}

void View::handler(const ResponseAnswer_ENUM cmd, const QString JSONdata)
{
    qDebug() <<"View::handler: input = "
             << cmd << " " << JSONdata;
    QJsonDocument doc;
    QJsonObject json;
    if(JSONdata!=""){
        doc = QJsonDocument::fromJson(JSONdata.toUtf8());
        json = doc.object();
        qDebug() << json["x"].toDouble() << " " << json["y"].toDouble();
    }
    switch (cmd) {
    case EYE_TRACKER_SUCESSFULL_INIT:
    {
        _menu->addEyeTrackerActions();
        break;
    }
    case CONTROLLER_ALL_GADGETS_SUCESSFULL_INITED:
        // todo!!!!
        break;
    case MODEL_INIT_COMRLETED:
    {
        _mainWindow->addAndShowInViewStack(_menu);
        break;
    }
    case MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET:
    {
        _eyeTrackerCalibrationWidget = new Calibration;
        connect(_eyeTrackerCalibrationWidget, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
        _mainWindow->addAndShowInViewStack(_eyeTrackerCalibrationWidget);
        break;
    }
    case VIEW_CALIBRATION_WIDGET_READY:
    {
        emit controllerHandler(VIEW_CALIBRATION_WIDGET_READY);
        break;
    }
    case MENU_START_CALIBRATION:
    {
        emit controllerHandler(VIEW_CALIBRATION_WIDGET_READY);
        break;
    }
    case EYE_TRACKER_POINT_TO_CALIBRATE:
    {
        _eyeTrackerCalibrationWidget->moveTo(json["x"].toDouble(), json["y"].toDouble(), json["time"].toInt());
        break;
    }
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION:
    {
        _mainWindow->showLoadingWidget();
        break;
    }
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED:
    {
        _mainWindow->closeLoadingWidget();
        break;
    }
    case EYE_TRACKER_LEAVE_CALIBRATION_MODE:
    {
        _mainWindow->showWidgetFromStack(_menu);
        delete _eyeTrackerCalibrationWidget;
        _eyeTrackerCalibrationWidget = nullptr;

        break;
    }
    default:
        break;
    }

}
