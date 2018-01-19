#ifndef TESTSCONTROLLER_H
#define TESTSCONTROLLER_H

#include <QObject>
#include "src/additional_header.h"

struct ROIs_TestModel{
    unsigned ID;
    ROI_type type;
    bool NOTforEyeTrackerFixation;
    QVector<QPoint> points;
    QPoint position;
    ROIs_TestModel():NOTforEyeTrackerFixation(false){}
};

class TestModel : public QObject
{
    Q_OBJECT
public:
    explicit TestModel(QObject *parent = nullptr);
    void setPath(const QString&){}
    void demo(){_demo = true;}
    ResponseAnswer_ENUM load();
signals:
    void loaded();
public slots:
public:
    // указатель на xml-file
    ResponseAnswer_ENUM loadTest(){} // реализовать
    void demoLoadROIs();

    bool _demo;
    QVector<ROIs_TestModel> _ROIsVector;
};

struct TestsListElement{
    int ID;
    BCI_TYPE _bciType;
    TestModel *_testModel;
    TestsListElement(int inID, BCI_TYPE inType, TestModel *inTestModel): ID(inID),
        _bciType(inType), _testModel(inTestModel) {}
};

class TestsController : public QObject
{
    Q_OBJECT
public:
    explicit TestsController(QObject *parent = nullptr);

    int init(); // todo
    int addBCI(BCI_TYPE);
    QList<ViewTestElement> getListOfTests();

    ResponseAnswer_ENUM loadTest(int ID);
    ResponseAnswer_ENUM startTest();
    QString getTestXMLfileLink(); // only after loadTest
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void testLoaded();
private:
    int activeTestID;
    int loadedPageID;
    QList<TestsListElement> _testsList;
    int findTests(BCI_TYPE); // реализваоть реальный поиск!!!!!
};

#endif // TESTSCONTROLLER_H
