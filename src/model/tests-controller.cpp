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
    qDebug() << "loadedPageID = " << loadedPageID;
    return MODEL_TESTS_CONTROLLER_LOADED;
}

ResponseAnswer_ENUM TestsController::startTest()
{
    if(activeTestID!=-1)
        return MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST;
    if(loadedPageID==-1)
        return MODEL_TESTS_CONTROLLER_NO_LOADED_TEST;
    activeTestID = loadedPageID;
    return MODEL_TESTS_CONTROLLER_SUCCESS_TEST_START;
}

ResponseAnswer_ENUM TestsController::finishTest()
{
    _testsList.at(activeTestID)._testModel->unload();
    activeTestID = -1;
    loadedPageID = -1;
    return OK_ANSWER;
}

QString TestsController::getTestXMLfileLink()
{
    return QString("demo");
}

int TestsController::getActiveTestID()
{
    return activeTestID;
}

void TestsController::setStreamForActiveTest(QTextStream *inStream)
{
    if(activeTestID == -1)
        return;
    _testsList.at(activeTestID)._testModel->setStream(inStream);
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

void TestsController::setGazePointForAnalysis(double inX, double inY)
{
    if(activeTestID == -1)
        return;
    _testsList.at(activeTestID)._testModel->setPointToROIsAnalysis(QPoint(inX*widgetSize.x(), inY*widgetSize.y()));
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
    _loaded = false;
}


ResponseAnswer_ENUM TestModel::load()
{
    if(_demo)
        demoLoadROIs(); // временная загрузка теста
    _loaded = true;
    return MODEL_TESTS_CONTROLLER_LOADED;

}

ResponseAnswer_ENUM TestModel::unload()
{
    _ROIsVector.clear();
    _loaded=false;
    return OK_ANSWER;
}

void TestModel::setPointToROIsAnalysis(QPoint inPoint)
{
    /// происходит проход по всем элементам вектора с ROI-облостями

    foreach (ROI_TestModel tmpROI, _ROIsVector) {
        if( (inPoint.x() >= tmpROI.position.x()) &&
                (inPoint.y() >= tmpROI.position.y()) )
        {
            if( (inPoint.x() <= tmpROI.points.at(0).x()) &&
                    (inPoint.y() <= tmpROI.points.at(0).y()) )
            {
                if(tmpROI.NOTforEyeTrackerFixation)
                    return;
                if(!outPutStream)
                    qDebug() << "Point in ROI with ID " << tmpROI.ID;
                else
                    *outPutStream << "ROI: " << tmpROI.ID << "\n"; // todo: необходимо модернизировать
            }
        }
    }

}

void TestModel::demoLoadROIs()
{
    ROI_TestModel tmpROI;
    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(318,384));
    tmpROI.position = QPoint(140,14);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(298,48));
    tmpROI.position = QPoint(475,14);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(197,50));
    tmpROI.position = QPoint(466,69);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(319,48));
    tmpROI.position = QPoint(502,125);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(437,103));
    tmpROI.position = QPoint(502,182);

   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(214,49));
    tmpROI.position = QPoint(502,293);

   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(211,50));
    tmpROI.position = QPoint(536,348);

   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(236,106));
    tmpROI.position = QPoint(595,403);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.NOTforEyeTrackerFixation = true;
    tmpROI.points.push_back(QPoint(65,104));
    tmpROI.position = QPoint(497,515);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(807,103));
    tmpROI.position = QPoint(497,625);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.points.push_back(QPoint(158,40));
    tmpROI.position = QPoint(495,740);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();

    tmpROI.ID = _ROIsVector.length();
    tmpROI.type = ROI_rect;
    tmpROI.NOTforEyeTrackerFixation = true;
    tmpROI.points.push_back(QPoint(27,47));
    tmpROI.position = QPoint(471,792);
   _ROIsVector.push_back(tmpROI);
    tmpROI.points.clear();
}
