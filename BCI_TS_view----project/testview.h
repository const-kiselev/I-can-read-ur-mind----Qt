#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QPainter>
#include <QFile>

enum ROI_type {ROI_rect};

//struct ViewTestElement{
//    int ID;
//    QString name;
//    BCI_TYPE _bciType;
//    QString converToJSONstr()
//    {
//        QJsonObject tmpJson;
//        tmpJson["ID"] = ID;
//        tmpJson["name"] = name;
//        tmpJson["BCI_TYPE"] = _bciType;
//        return JSONtoStr(tmpJson);
//    }
//};

struct ROI_VectorElement{
    unsigned ID;
    ROI_type type;
    bool NOTforEyeTrackerFixation;
    QVector<QPointF> points;
    QPointF position;
    QString content;
    QString style; // ToDo: посмотреть, мб в Qt есть тип связанный со стилем lable
    ROI_VectorElement():NOTforEyeTrackerFixation(false){}
};

struct TestRepresinatationData{
    QVector<ROI_VectorElement> ROIsVector;
    QRect dividedLine;
    QString testStyle;
};

class TestView : public QWidget
{
    Q_OBJECT
public:
    TestView(QWidget *parent = nullptr);
    int load(QFile*); // changed
signals:
    void reload();
    void loadNew();
private:
    bool testLoaded;
    QFile *xmlFile; // new. Delete QString address;

    void demoLoad();
    void draw();
    void drawROI(ROI_VectorElement &inROI);
    void readXML();
    bool _demo;
    TestRepresinatationData *_testView;
};

#endif // TESTVIEW_H
