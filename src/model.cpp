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
    qDebug() << "Model::handler recieved: "<< cmd;
    switch (cmd) {
    case MODEL_INIT_ALL_GADGETS:
    {
        _eyeTracker = new EyeTracker;
        qRegisterMetaType<ResponseAnswer_ENUM>();
        connect(_eyeTracker, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)),
                Qt::QueuedConnection);
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
    case CONTROLLER_START_TEST_d:
    {

        break;
    }
    default:
        break;
    }
}

void Model::init()
{
    _testsController = new TestsController;
    if(_testsController->init()){ // инициализация контроллера тестов, если неуспешно, то выходим
        delete _testsController;
        _testsController = nullptr;
        emit viewHandler(ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT);
        qDebug() << "ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT";
        break;
    }
    connect(_testsController, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
            this, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    if(!_eyeTracker->init()) // инициализация айТрекера
    {
        emit viewHandler(EYE_TRACKER_SUCESSFULL_INIT);
        if(_testsController->addBCI(BCI_TYPE_EYE_TRACKER)){ // если хотя бы один тест был найден, то выполняем
            QList tmp = _testsController->getListOfTests(); // запрашиваем список с доступными тестами
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
