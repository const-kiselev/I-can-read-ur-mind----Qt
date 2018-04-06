#include "model.h"
Model::Model(QObject *parent) : QObject(parent)
{
    _eyeTracker = nullptr;
    _testsController = nullptr;
    _file = nullptr;
    _textFileStream = nullptr;
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
        if(cmd !=  MODEL_SET_GAZE_POINT_DESTINATION_notJSON_d)
        {
            doc = QJsonDocument::fromJson(JSONdata.toUtf8());
            json = doc.object();
        }
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
        // инициализируем и добавляем тесты
        init();
        emit controllerHandler(MODEL_INIT_COMRLETED);
        //emit viewHandler(MODEL_INIT_COMRLETED);
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
        // загружаем тест из контролера тестов
        resp = _testsController->loadTest(json["ID"].toInt());
        emit controllerHandler(resp);
        if(resp != MODEL_TESTS_CONTROLLER_LOADED)
            break;
        QJsonObject respJson;
        respJson["testPath"] = _testsController->getTestXMLfileLink();
        emit controllerHandler(MODEL__TEST_PATH_d, JSONtoStr(respJson));
        break;
    }
    case VIEW_TEST_VIEW_SHOW_SUCCESS:
    {
        qDebug() << _testsController->startTest();
            /// открываем новый файл для записи RAW-данных трекинга
        // todo: после добавления класса с юзером, необходимо будет указать его ID в файле!
        QString userName = "";
        if(_admin)
            userName = _admin->getName();
        openFile(QDateTime::currentDateTime().toString("yy_MM_dd_hh_mm_ss_zzz_")+_testsController->getTestXMLfileLink()+"_"+userName);
        setStreamForTracking(); // устанавливаем для айТрекера стрим для вывода RAW данных непосредственно в файл
            /// соединяем сигнал об изменении GazePoint с контроллером тестов, который определяет алгоритм анализа данных
        connect(_eyeTracker, SIGNAL(sendGazePoint(double,double)),
                _testsController, SLOT(setGazePointForAnalysis(double,double)));
            /// запускаем трекинг в другом потоке
        _testsController->setStreamForActiveTest(_textFileStream);
        QtConcurrent::run(_eyeTracker, &EyeTracker::startTracking);
        break;
    }
    case VIEW_TEST_CLOSE_TEST:
    {
        _eyeTracker->stopTracking();
        // TODO: необходимо реализовать выход из теста!
        disconnect(_eyeTracker, SIGNAL(sendGazePoint(double,double)),
                   _testsController, SLOT(setGazePointForAnalysis(double,double)));
        _testsController->finishTest();
        closeFile();
        break;
    }
    case VIEW_WINDOW_SIZE_d:
    {
        _testsController->setWidgetSize(json["width"].toInt(),
                json["height"].toInt());
        break;
    }
    case MODEL_ADD_TEST_d:
    {
        _testsController->addTest(json["fileName"].toString());
        break;
    }
    case MODEL_TEST_WAS_ADDED_d:
    {
        emit controllerHandler(MODEL_TEST_WAS_ADDED_d, JSONdata);
        break;
    }
    case MODEL_TEST_WAS_DELETED_d:
    {
        // TODO: реализовать
        break;
    }
    case MODEL_SET_GAZE_POINT_DESTINATION_notJSON_d:
    {
        //gazePointDestination = (ResponseAnswer_ENUM)JSONdata.toInt();
        gazePointDestination = MODEL_SET_GAZE_POINT_DESTINATION_notJSON_d;
        break;
    }
    case EYE_TRACKER_START_TRACKING:
    {
        openFile(QDateTime::currentDateTime().toString("yy_MM_dd_hh_mm_ss_zzz_")+QString("GazePointTest"));
        setStreamForTracking(); // устанавливаем для айТрекера стрим для вывода RAW данных непосредственно в файл
            /// соединяем сигнал об изменении GazePoint с контроллером тестов, который определяет алгоритм анализа данных
        connect(_eyeTracker, SIGNAL(sendGazePoint(double,double)),
                this, SLOT(gazePoint(double,double)));
        emit controllerHandler(EYE_TRACKER_STARTED_TRACKING);
        /// запускаем трекинг в другом потоке
        QtConcurrent::run(_eyeTracker, &EyeTracker::startTracking);
        break;
    }
    case EYE_TRACKER_STOP_TRACKING:
    {
        _eyeTracker->stopTracking();
        disconnect(_eyeTracker, SIGNAL(sendGazePoint(double,double)),
                   this, SLOT(gazePoint(double,double)));
        gazePointDestination = OK_ANSWER;
        closeFile();
        break;
    }
    case MODEL_ADMIN_MODULE_INIT:
    {

        if(_admin)
        _admin->setTestList(_testsController->getListOfTests());
        break;
    }
    case MODEL_ADMIN_MODULE_GET_DATA:
    {
        if(!_admin->isInited())
        {
            emit handler(MODEL_ADMIN_MODULE_INIT);
            _admin->changeInited();
        }
        emit controllerHandler(MODEL_ADMIN_MODULE_DATA_d, JSONtoStr(_admin->getAllFields()));
        break;
    }
    case MODEL_ADMIN_MODULE_UPDATE_DATA_d:
    {
        _admin->updateFields(JSON_fromStr(JSONdata));
        break;
    }
    case MODEL_ADMIN_NEXT_TEST:
    {
        int res = _admin->getNextTestID();
        if(res!=-1){
            QJsonObject tmpJson;
            tmpJson["ID"] = res;
            handler(MODEL_START_TEST_d, JSONtoStr(tmpJson));
        }
        else
            emit controllerHandler(MODEL_ADMIN_THERE_IS_NO_TEST);
        break;
    }
    case MODEL_TESTS_CONTROLLER_SET_ANSWERS_d:
    {
        *_textFileStream << JSONdata << "\n";
        break;
    }
    default:
        break;
    }
}

void Model::gazePoint(double inX, double inY)
{
    if(gazePointDestination == OK_ANSWER)
        return;
    if(gazePointDestination>VIEW)
        emit viewHandler(MODEL_RAW_GAZE_POINT_notJSON_d, QString::number(inX)+" "+QString::number(inY));
    else
    {
        // handler(gazePointDestination, someJSON);
        // реализовать!!!
    }

}

void Model::init()
{
    _testsController = new TestsController();
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
        _testsController->addBCI(BCI_TYPE_EYE_TRACKER);
//        if(_testsController->addBCI(BCI_TYPE_EYE_TRACKER)){ // если хотя бы один тест был найден, то выполняем
//            QList<ViewTestElement> tmp = _testsController->getListOfTests(); // запрашиваем список с доступными тестами
//            foreach (ViewTestElement tmpElement, tmp) { // проходимся по всем тестам списка
//                emit viewHandler(MENU_ADD_TEST_d, tmpElement.converToJSONstr()); // отправляем меню данные о тесте
//            }
//        }
    }
    else{
        emit viewHandler(ERROR_MODEL_EYE_TRACKER_INIT);
        qDebug() << "ERROR_MODEL_EYE_TRACKER_INIT";
    }
    if(ADMIN_MODULE){
        _admin = new Admin();
        emit controllerHandler(MODEL_ADMIN_MODULE_CREATED);
        // todo: необходимо отсылать сообщение о добавлении в меню
    }
}
void Model::openFile(QString fileName)
{
    if(_file)
    {
        qDebug() << "There's open file. Please, close it for new one.";
        return;
    }
    _file = new QFile(fileName + ".txt");
    if(!_file->open(QIODevice::ReadWrite))
    {
        qDebug() << "Can not open the file.";
        delete _file;
        _file = nullptr;
        return;
    }
    qDebug() << "File Name: " << fileName;
    _textFileStream = new QTextStream(_file);
}

void Model::closeFile()
{
    delete _textFileStream;
    _textFileStream = nullptr;
    _file->close();
    delete _file;
    _file = nullptr;
}

void Model::setStreamForTracking()
{
    _eyeTracker->setTextStream(_textFileStream);
}
