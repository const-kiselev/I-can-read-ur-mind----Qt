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
private:
    bool testLoaded;
    QString address;

    void demoLoad();
    void draw();
    bool _demo;
    TestRepresinatationData *_testView;
};

#endif // TESTVIEW_H
