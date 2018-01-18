#ifndef TESTSCONTROLLER_H
#define TESTSCONTROLLER_H

#include <QObject>
#include "src/additional_header.h"

class TestModel : public QObject
{
    Q_OBJECT
public:
    explicit TestModel(QObject *parent = nullptr);
    void setPath(const QString&){}
    void demo(){_demo = true;};
    void loadingForShow();
    void demoLoadingForShow();
signals:
    void loaded();
public slots:
public:
    // указатель на xml-file
    void loadTest(){} // реализовать
    void demoLoad();
    bool _demo;
    TestRepresinatationData *_testView;
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
    void startTest(int ID);
    QString *getTestLinkForView(); // only after loadTest
    QList<ViewTestElement> getListOfTests();
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void testLoaded();
private:
    int activeTestID;
    int loadPageID;
    QList<TestsListElement> _testsList;
    int findTests(BCI_TYPE); // реализваоть реальный поиск!!!!!
    int loadTest(int ID); // todo: make response if there was a problem with loading
};

#endif // TESTSCONTROLLER_H
