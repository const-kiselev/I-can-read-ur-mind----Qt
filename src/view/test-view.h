#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QPainter>
#include "src/additional_header.h"

class TestView : public QWidget
{
    Q_OBJECT
public:
    TestView(QWidget *parent = nullptr);
    ResponseAnswer_ENUM load(QString);
    int draw();
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
private:
    bool testLoaded;
    QFile *xmlFile; // new. Delete QString address;
    QQueue<QWidget*> queueOfProblemsWidgets, queueOfAnswersWidgets;
    QList<QWidget*> listOfAnswerFields;

    void demoLoad();
    bool _demo;
    TestRepresinatationData *_testView;
    void drawArea(ROI_VectorElement &inROI, bool inEntryForm);
    void readXML();
    void readProblemTag(QXmlStreamReader&xml);
    bool first;
    void clearLayout();
    QJsonObject getAllInputFieldsInWidget(QWidget *wdg, bool answerWidget=false);

    QStackedWidget *stWidget;
};

#endif // TESTVIEW_H
