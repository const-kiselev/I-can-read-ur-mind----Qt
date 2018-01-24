#include "model.h"
Model::Model(QObject *parent) : QObject(parent)
{
    _eyeTracker = nullptr;
    _testsController = nullptr;
}

Model::~Model()
{
    delete _eyeTracker;
    delete _testsController;
}

void Model::handler(const ResponseAnswer_ENUM cmd, const QString&JSONdata)
{
    qDebug() << "Model::handler recieved: "
             << cmd << " " << JSONdata;
    QJsonDocument doc;
    QJsonObject json;
    ResponseAnswer_ENUM resp;
    if(JSONdata!=""){
        doc = QJsonDocument::fromJson(JSONdata.toUtf8());
        json = doc.object();
    }
    switch (cmd) {
    case MODEL_INIT_ALL_GADGETS:
    {
        _eyeTracker = new EyeTracker;
        qRegisterMetaType<ResponseAnswer_ENUM>();
        connect(_eyeTracker, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)),
                Qt::QueuedConnection);
        connect(_eyeTracker, SIGNAL(sendGazePoint(double,double)),
                this, SLOT(gazePoint(double,double)));
        break;
    }
    case MODEL_INIT:
    {
        init();
        emit viewHandler(MODEL_INIT_COMRLETED);
        break;
    }
    case EYE_TRACKER_START_CALIBRATION:
    {
        // работает с помощью интсрумента управления потоками. То есть, данная функция запускается в другом потоке
        QtConcurrent::run(_eyeTracker, &EyeTracker::calibrate);
        break;
    }
    case EYE_TRACKER_POINT_TO_CALIBRATE:
    {
        emit viewHandler(EYE_TRACKER_POINT_TO_CALIBRATE, JSONdata);
        break;
    }
    case EYE_TRACKER_LEAVE_CALIBRATION_MODE:{
        emit viewHandler(EYE_TRACKER_LEAVE_CALIBRATION_MODE);
        break;
    }
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION:
    {
        emit viewHandler(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION);
        break;
    }
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED:
    {
        emit viewHandler(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED);
        break;
    }
    case TESTS_CONTROLLER_LIST_WITH_TESTS_WAS_UPD:
        // todo
        break;
    case MODEL_START_TEST_d:
    {
        resp = _testsController->loadTest(json["ID"].toInt());
        emit controllerHandler(resp);
        if(resp != MODEL_TESTS_CONTROLLER_LOADED)
            break;
        QString tmpRespStrT;
        QJsonObject respJson;
        respJson["testPath"] = _testsController->getTestXMLfileLink();
        emit controllerHandler(MODEL__TEST_PATH_d, JSONtoStr(respJson));
        break;
    }
    case VIEW_TEST_VIEW_SHOW_SUCCESS:
    {
        //запускаем трегинг в другом потоке
        QtConcurrent::run(_eyeTracker, &EyeTracker::startTracking);
        break;
    }
    default:
        break;
    }
}

void Model::gazePoint(double inX, double inY)
{

}

void Model::init()
{
    _testsController = new TestsController;
    if(_testsController->init()){ // инициализация контроллера тестов, если неуспешно, то выходим
        delete _testsController;
        _testsController = nullptr;
        emit viewHandler(ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT);
        qDebug() << "ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT";
        return;
    }
    connect(_testsController, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    if(!_eyeTracker->init()) // инициализация айТрекера
    {
        emit viewHandler(EYE_TRACKER_SUCESSFULL_INIT);
        if(_testsController->addBCI(BCI_TYPE_EYE_TRACKER)){ // если хотя бы один тест был найден, то выполняем
            QList<ViewTestElement> tmp = _testsController->getListOfTests(); // запрашиваем список с доступными тестами
            foreach (ViewTestElement tmpElement, tmp) { // проходимся по всем тестам списка
                emit viewHandler(MENU_ADD_TEST_d, tmpElement.converToJSONstr()); // отправляем меню данные о тесте
            }
        }
    }
    else{
        emit viewHandler(ERROR_MODEL_EYE_TRACKER_INIT);
        qDebug() << "ERROR_MODEL_EYE_TRACKER_INIT";
    }
}
