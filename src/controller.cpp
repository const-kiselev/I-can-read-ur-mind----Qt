#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{

}

void Controller::handler(const ResponseAnswer_ENUM cmd, const QString JSONdata)
{
    qDebug() << "Controller::handler recieved: "<< cmd;
    switch (cmd) {
    case VIEW_INIT_COMRLETED:
    {
        emit modelHandler(MODEL_INIT_ALL_GADGETS);
        // после инициализации всех н-интерфейсов необходимо найти (загрузить) все доступные тесты в рабочей директории
        QJsonObject * tmpJson = findAllTests();
        // конвертируем список в JSON
        emit modelHandler(MODEL_INIT_d, JSONtoStr(*tmpJson));
        delete tmpJson;
        break;
    }
    case VIEW_CALIBRATION_WIDGET_READY:
    {
        emit modelHandler(EYE_TRACKER_START_CALIBRATION);
        break;
    }
    case MENU_OPEN_TEST_d:
    {
        emit modelHandler(MODEL_START_TEST_d, JSONdata);
        break;
    }
//    case MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED:
//    case MODEL_TESTS_CONTROLLER_SUCCESS:
//    case MODEL_TESTS_CONTROLLER_NO_SUCH_TEST_ID:
//    case MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED:
//    case MODEL_TESTS_CONTROLLER_LOADED:
//    case MODEL_TESTS_CONTROLLER_NO_LOADED_TEST:
//    case MODEL_TESTS_CONTROLLER_SUCCESS_TEST_START:
//    case MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST:
//        break;
    case MODEL__TEST_PATH_d:
    {
        emit viewHandler(VIEW_TEST_PATH_FOR_LOAD, JSONdata);
        break;
    }
    case VIEW_TEST_VIEW_SHOW_SUCCESS:
    {// отправляем запрос на запуск трекинга
        emit modelHandler(VIEW_TEST_VIEW_SHOW_SUCCESS);
        break;
    }
    case VIEW_WINDOW_SIZE_d:
    {
        emit modelHandler(VIEW_WINDOW_SIZE_d, JSONdata);
        break;
    }
    case VIEW_TEST_CLOSE_TEST:
    {
        emit modelHandler(VIEW_TEST_CLOSE_TEST);
        break;
    }
    case APP_EXIT:
        emit appExit();
        break;
    default:
        break;
    }
}

QJsonObject *Controller::findAllTests()
{
    qDebug() << "QDir::current()" << QDir::current();
    workDir = QDir::current();
    // находим в актуальной директории все файлы с помощью фильтра
    listFiles = workDir.entryList("BCIs_T*.xml", QDir::Files);
    QJsonObject * tmpJson = new QJsonObject;

    int i=0;
    foreach(QString fileElement, listFiles)
        tmpJson[QString(i)] = fileElement;
    return tmpJson;
}
