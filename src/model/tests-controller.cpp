#include "tests-controller.h"

TestsController::TestsController(QObject *parent) : QObject(parent)
{
    activeTestID = -1;
    loadedPageID = -1;
}

int TestsController::init()
{
    return 0;
}

int TestsController::addBCI(BCI_TYPE in_bciType)
{
    // Поиск доступных тестов
    return findTests(in_bciType);
}

ResponseAnswer_ENUM TestsController::loadTest(int ID) // todo: посмотреть
{
    if(activeTestID!=-1)
        return MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST;
    if(_testsList.length()<=ID || ID<0)
        return MODEL_TESTS_CONTROLLER_NO_SUCH_TEST_ID;
    connect(_testsList.at(ID)._testModel, SIGNAL(loaded()), this, SLOT(testLoaded()));
    if (_testsList.at(ID)._testModel->load()!=MODEL_TESTS_CONTROLLER_LOADED) // загрузка данных ROIs для использования в потоковом анализе данных айТрекера
        return MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED;
    loadedPageID = ID;
    return MODEL_TESTS_CONTROLLER_LOADED;
}

ResponseAnswer_ENUM TestsController::startTest()
{
    if(activeTestID!=-1)
        return MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST;
    if(loadedPageID=-1)
        return MODEL_TESTS_CONTROLLER_NO_LOADED_TEST;
    activeTestID = loadedPageID;
    return MODEL_TESTS_CONTROLLER_SUCCESS_TEST_START;
}

QString TestsController::getTestXMLfileLink()
{
    return QString("demo");
}

int TestsController::getActiveTestID()
{
    return activeTestID;
}

QList<ViewTestElement> TestsController::getListOfTests()
{
    QList<ViewTestElement> res;
    ViewTestElement tmp;
    int len = _testsList.length();
    for(int i=0; i<len; i++){
        tmp.ID = _testsList.at(i).ID;
        tmp.name = "ТЕСТ 1";
        tmp._bciType = _testsList.at(i)._bciType;
        res.push_back(tmp);
    }
    return res;
}

void TestsController::testLoaded()
{

}

int TestsController::findTests(BCI_TYPE in_bciType)
{
    TestModel *tmpTestModel = new TestModel;
    tmpTestModel->demo();
    _testsList << TestsListElement(_testsList.length(), in_bciType, tmpTestModel);
    return 1; // временно. Данная функция должна возвращать количсевто найденных тестов!
}

////////////////////////////////////////////////

TestModel::TestModel(QObject *parent): QObject(parent)
{
    _demo = false;
}


ResponseAnswer_ENUM TestModel::load()
{
    if(_demo)
        demoLoadROIs(); // временная загрузка теста
    return MODEL_TESTS_CONTROLLER_LOADED;

}

void TestModel::demoLoadROIs()
{

}
