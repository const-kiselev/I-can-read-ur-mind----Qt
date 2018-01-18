#include "tests-controller.h"

TestsController::TestsController(QObject *parent) : QObject(parent)
{
    activeTestID = -1;
    loadPageID = -1;
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

int TestsController::loadTest(int ID) // todo: посмотреть
{
    connect(_testsList.at(ID)._testModel, SIGNAL(loaded()), this, SLOT(testLoaded()));
    _testsList.at(ID)._testModel->loadingForShow();
    return 0;
}

QString *TestsController::getTestLinkForView()
{
    return new QString("HELLO");
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

TestModel::TestModel(QObject *parent)
{
    _demo = false;
}


void TestModel::loadingForShow()
{
    if(_demo)
        demoLoadingForShow();

}

void TestModel::demoLoadingForShow()
{
    _testView = new TestRepresinatationData;
    ROI_VectorElement tmpROI;
    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(140+318,14+384));
    tmpROI.position = QPoint(140,14);
    tmpROI.content = "Q1. This program will indicate whether an input number is a perfect number (a positive integer that is equal to the sum of its proper positive divisors, e.g., 6 is a perfect number because 6=1+2+3). Please find three possible bigs in it.";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(475+298,14+48));
    tmpROI.position = QPoint(475,14);
    tmpROI.content = "#include <stdio.h>";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(466+197,69+50));
    tmpROI.position = QPoint(466,69);
    tmpROI.content = "int main(){";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(502+319,125+48));
    tmpROI.position = QPoint(502,125);
    tmpROI.content = "int n, i=1, sum=0;";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(502+437,182+103));
    tmpROI.position = QPoint(502,182);
    tmpROI.content = "printf(\"Enter a number: \"); scanf(\"%d\",n);";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(502+214,293+49));
    tmpROI.position = QPoint(502,293);
    tmpROI.content = "while(i<=n){";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(536+211,348+50));
    tmpROI.position = QPoint(536,348);
    tmpROI.content = "if(i%n==0){";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(595+236,403+106));
    tmpROI.position = QPoint(595,403);
    tmpROI.content = "sum=sum+i;i++;";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.NOTforEyeTrackerFixation = true;
    tmpROI.points.push_back(QPoint(497+65,515+104));
    tmpROI.position = QPoint(497,515);
    tmpROI.content = "}}";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(497+807,625+103));
    tmpROI.position = QPoint(497,625);
    tmpROI.content = "if(sum==n)   printf(\“%d is a perfect number\”, i); else   print(\“%d is not a perfect number\”);";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.points.push_back(QPoint(497+807,625+103));
    tmpROI.position = QPoint(497,625);
    tmpROI.content = "return 0;";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);

    tmpROI.ID = _testView->ROIsVector.length();
    tmpROI.type = rect;
    tmpROI.NOTforEyeTrackerFixation = true;
    tmpROI.points.push_back(QPoint(471+27,792+47));
    tmpROI.position = QPoint(471,792);
    tmpROI.content = "}";
    tmpROI.style = "";
    _testView->ROIsVector.push_back(tmpROI);
}
