#include "view.h"

View::View(QObject *parent) : QObject(parent)
{
    // ADD resize!!!!
    // FOR macOS:
    //QTimer::singleShot(1000, &window, SLOT(showFullScreen()));
    _gazePointTest = nullptr;

}

void View::init()
{
    _mainWindow = new MainWindow();
    connect(_mainWindow, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    _mainWindow->init();
    _mainWindow->show();
    _mainWindow->showFullScreen();
    _mainWindow->showLoadingWidget();
    _menu = new Menu();
    _menu->addItem("Выйти", APP_EXIT);
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
    if(JSONdata!="" && cmd!=MODEL_RAW_GAZE_POINT_notJSON_d){
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
        _eyeTrackerCalibrationWidget = new Calibration();
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
        _mainWindow->removeFromStack(_eyeTrackerCalibrationWidget);
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
    case VIEW_SHOW_ADDED_WIDGET:
    {
        _mainWindow->showLastAddedWidget();
        break;
    }
    case EYE_TRACKER_STARTED_TRACKING:
    {
        if(_gazePointTest)
            handler(VIEW_SHOW_ADDED_WIDGET);
        break;
    }
    case MENU_OPEN_GAZE_POINT_TEST_WIDGET:
    {
        _gazePointTest = new GazePointTest();
        // some button connect
        _mainWindow->showLoadingWidget();
        _mainWindow->addInViewStack(_gazePointTest);
        _mainWindow->addButton("Выйти", QPoint(0,0), _gazePointTest, false, VIEW_CLOSE_GAZE_POINT_TEST);
        // sending the signal to controller that we're ready to open widget
        emit controllerHandler(VIEW_GAZE_POINT_TEST_WIDGET_READY);
        break;
    }
    case MODEL_RAW_GAZE_POINT_notJSON_d: // пока используется только для GazePointTest
    {
        if(!_gazePointTest)
            return;
        int spacePos = JSONdata.indexOf(" ");
        double xValue = JSONdata.mid(0,spacePos).toDouble(),
                yValue = JSONdata.mid(spacePos+1, JSONdata.length()-spacePos-1).toDouble();
        qDebug() << "MODEL_RAW_GAZE_POINT_notJSON_d x = " << xValue << " " << yValue;
        _gazePointTest->moveTo(xValue, yValue);
        break;
    }
    case VIEW_CLOSE_GAZE_POINT_TEST:
    {
        if(!_gazePointTest)
            return;
        emit controllerHandler(VIEW_CLOSE_GAZE_POINT_TEST);
        _mainWindow->removeFromStack(_gazePointTest);
        delete _gazePointTest;
        _gazePointTest = nullptr;
        break;
    }
    case MENU_ADD_ADMIN_BUTTON:
    {
        _menu->addItem("Администрирование", MENU_OPEN_ADMIN_MODULE);
        break;
    }
    case MENU_OPEN_ADMIN_MODULE:
    {
        _mainWindow->showLoadingWidget();
        _adminView = new AdminView();
        emit controllerHandler(MENU_OPEN_ADMIN_MODULE);
        connect(_adminView, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
        break;
    }
    case VIEW_ADMIN_MODULE_DATA_d:
    {
        if(_adminView ->addData(JSONdata) == VIEW_ADMIN_MODULE_LOADED)
        {
            _adminView->draw();
            _mainWindow->addInViewStack(_adminView);
            _mainWindow->addButton("Выйти", QPoint(0,0), _adminView, false, VIEW_ADMIN_MODULE_CLOSE_WIDGET);
            handler(VIEW_SHOW_ADDED_WIDGET);
        }
        break;
    }
    case VIEW_ADMIN_MODULE_CLOSE_WIDGET:
    {
        if(!_adminView)
            return;
        emit controllerHandler(VIEW_ADMIN_MODULE_UPDAT_DATA_d, JSONtoStr(_adminView->getFieldsContent()));
        _mainWindow->removeFromStack(_adminView);
        delete _adminView;
        _adminView = nullptr;
        break;
        // todo: !!!!!
    }
    case VIEW_ADMIN_MODULE_PRESSED_START_TEST_SESSION:
    {
        handler(VIEW_ADMIN_MODULE_CLOSE_WIDGET);
        emit controllerHandler(VIEW_ADMIN_MODULE_START_TEST_SESSION);
        break;
    }
    case VIEW_OPEN_TEST_SESSION_MENU:
    {
        _testSessionMenu = new Menu();
        _testSessionMenu->resize(_mainWindow->width(), _mainWindow->height());
        QLabel *greetings = new QLabel("Привет! Перед тобой будет пресдтавлена серия тестов, результаты которого помогут в научно-исследовательской работе. Не бойся, это никак не повлияет на твою успеваемость, даю слово!:) Если будут интересны результаты, просто об этом скажи администратору тестирования.\nНа каждом кадре будет инструкция, так что ты не запутаешься. \nУдачи!");
        greetings->setMaximumWidth(500);
        greetings->setWordWrap(true);
        greetings->setFont(QFont( "Arial", 14, QFont::Normal));
        greetings->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        _testSessionMenu->addWidget(greetings);
        _testSessionMenu->addItem("Я готов!", VIEW_TEST_SESSION_CALIBRATION);
        _mainWindow->addInViewStack(_testSessionMenu);
        connect(_testSessionMenu, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
        handler(VIEW_SHOW_ADDED_WIDGET);
        break;
    }
    case VIEW_TEST_SESSION_CALIBRATION:
    {
        handler(MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET);
        _testSessionMenu->clear();
        QLabel *greetings = new QLabel("Ну как? Продолжим?");
        greetings->setMaximumWidth(500);
        greetings->setWordWrap(true);
        greetings->setFont(QFont( "Arial", 14, QFont::Normal));
        greetings->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        _testSessionMenu->addWidget(greetings);
        _testSessionMenu->addItem("Да!", VIEW_TEST_SESSION_MENU_READY_FOR_TEST);
        break;
    }
    case VIEW_TEST_PRESSED_NEXT:
    {
        if(_testView->draw())
            handler(VIEW_TEST_CLOSE_TEST);
        break;
    }
    case VIEW_TEST_SESSION_MENU_READY_FOR_TEST:
    {
        emit controllerHandler(VIEW_TEST_SESSION_MENU_READY_FOR_TEST);
        break;
    }
    case VIEW_TEST_SESSION_MENU_CLOSE:
    {
        if(!_testSessionMenu)
            return;
        //emit controllerHandler(VIEW_ADMIN_MODULE_UPDAT_DATA_d, JSONtoStr(_adminView->getFieldsContent()));
        _mainWindow->removeFromStack(_testSessionMenu);
        delete _testSessionMenu;
        _testSessionMenu = nullptr;
        break;
    }
    case VIEW_TEST_ANSWERS_d:
    {
        emit controllerHandler(VIEW_TEST_ANSWERS_d, JSONdata);
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
