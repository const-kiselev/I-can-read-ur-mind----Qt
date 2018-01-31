#ifndef TESTSCONTROLLER_H
#define TESTSCONTROLLER_H

#include <QObject>
#include "src/additional_header.h"



struct ROI_TestModel{
    unsigned ID;
    ROI_type type;
    bool NOTforEyeTrackerFixation;
    QVector<QPointF> points;
    QPointF position;
    ROI_TestModel():NOTforEyeTrackerFixation(false){}
};

class TestModel : public QObject
{
    Q_OBJECT
public:
    explicit TestModel(QObject *parent = nullptr);
    void setPath(QFile *inFile){xmlFile = inFile;}
    ResponseAnswer_ENUM load();
    ResponseAnswer_ENUM unload();
    void setStream(QTextStream *inStream) {outPutStream = inStream;}
signals:
    void loadedSignal();
public slots:
    /// Будем получать данные уже в конвертированном виде,
    /// так как это не копетенция самой модели теста
    void setPointToROIsAnalysis(QPoint inPoint);
private:
    // указатель на xml-file
    ResponseAnswer_ENUM loadTest(){} // реализовать
    // Загружает ROI-области для анализа демо теста
    //void demoLoadROIs();
    void readXML();
    QFile *xmlFile;
    bool loaded;
    bool _demo;
    // вектор ROI-областей
    QVector<ROI_TestModel> _ROIsVector;
    QTextStream *outPutStream;

};

struct TestFileData{
    unsigned ID;
    QFile * file;
    QString name;
    QList<BCI_TYPE> neededBCIs;
    TestModel *testModel;
    TestFileData():file(nullptr), testModel(nullptr), name(""){}
};

struct TestsListElement{
    int ID;
    BCI_TYPE _bciType;
    TestModel *_testModel;
    TestsListElement(int inID, BCI_TYPE inType, TestModel *inTestModel): ID(inID),
        _bciType(inType), _testModel(inTestModel) {}
};

/*!
    \class TestsController
    \brief  The TestsController class is tool for tests managment
 */



class TestsController : public QObject
{
    Q_OBJECT
public:
    TestsController(QObject *parent = nullptr);

    int init(); // todo
    int addBCI(BCI_TYPE);
    QList<ViewTestElement> getListOfTests();
    int addTest(QString FileName);

    ResponseAnswer_ENUM loadTest(int ID);
    ResponseAnswer_ENUM startTest();
    ResponseAnswer_ENUM finishTest();
    QString getTestXMLfileLink(); // only after loadTest
    int getActiveTestID();
    void setWidgetSize(int inW, int inH) {
        widgetSize.setX(inW);
        widgetSize.setY(inH);
    }
    void setStreamForActiveTest(QTextStream *inStream);
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void testLoaded();
    void setGazePointForAnalysis(double inX, double inY);
private:
    int activeTestID;
    int loadedPageID;

    QList<TestFileData> testsFilesList;
    QList<BCI_TYPE> activeBCIs;
    void addTestModel(int ID);
    void deleteTestModel(int ID);
    QPoint widgetSize;
    //QList<TestsListElement> _testsList;
//    int findTests(BCI_TYPE) // Deleted
    void checkTestsAndBCIs(); // проверка доступных тестов с учетом доступных нейроинтерфейсов
};

#endif // TESTSCONTROLLER_H
