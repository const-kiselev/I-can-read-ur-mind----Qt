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
    _mainWindow->showFullScreen();
    _mainWindow->showLoadingWidget();
    _menu = new Menu;
    qDebug() << "_mainWindow width = " << _mainWindow->width() << " height = " << _mainWindow->height();
    _menu->resize(_mainWindow->width(), _mainWindow->height());
    connect(_menu, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    emit controllerHandler(VIEW_INIT_COMRLETED);

}

void View::handler(const ResponseAnswer_ENUM cmd, const QString JSONdata)
{
    qDebug() <<"View::handler: input = "
             << cmd << " " << JSONdata;
    QJsonDocument doc;
    QJsonObject json;
    ResponseAnswer_ENUM resp;
    if(JSONdata!=""){
        doc = QJsonDocument::fromJson(JSONdata.toUtf8());
        json = doc.object();
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
    case MENU_ADD_TEST_d:
    {
        QJsonObject tmpJson;
        tmpJson["ID"] = json["ID"];
        _menu->addItem(json["name"].toString(), MENU_OPEN_TEST_d, JSONtoStr(tmpJson));
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
        qDebug() << json["x"].toDouble() << " " << json["y"].toDouble();
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
    case MENU_OPEN_TEST_d:
    {
        _mainWindow->showLoadingWidget();
        emit controllerHandler(MENU_OPEN_TEST_d, JSONdata);
        break;
    }
    case VIEW_TEST_PATH_FOR_LOAD:
    {
        _testView = new TestView();
        _testView->resize(_mainWindow->size());
        resp = _testView->load(json["testPath"].toString());
        emit controllerHandler(resp);
        if(resp!=VIEW_TEST_VIEW_LOAD_SUCCESS)
        {
            delete _testView;
            _testView = nullptr;
            break;
        }
        _mainWindow->addAndShowInViewStack(_testView);
        QJsonObject tmpSizeJson;
        tmpSizeJson["width"] = _mainWindow->width();
        tmpSizeJson["height"] = _mainWindow->height();
        emit controllerHandler(VIEW_WINDOW_SIZE_d, JSONtoStr(tmpSizeJson));
        emit controllerHandler(VIEW_TEST_VIEW_SHOW_SUCCESS);

        connect(_testView, SIGNAL(sendSignal(ResponseAnswer_ENUM,QString)),
                this, SLOT(handler(ResponseAnswer_ENUM,QString)));
        break;
    }
    case VIEW_TEST_CLOSE_TEST:
    {
        emit controllerHandler(VIEW_TEST_CLOSE_TEST);
        disconnect(_testView, SIGNAL(sendSignal(ResponseAnswer_ENUM,QString)),
                   this, SLOT(handler(ResponseAnswer_ENUM,QString)));
        _mainWindow->removeFromStack(_testView);
        delete _testView;
        _testView = nullptr;
        break;
    }
    case APP_EXIT:
    {
        emit controllerHandler(APP_EXIT);
        break;
    }
    default:
        break;
    }

}
