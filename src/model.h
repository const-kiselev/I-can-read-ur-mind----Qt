#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QDateTime>

#include "src/additional_header.h"
#include "src/model/eye-tracker.h"
#include "src/model/tests-controller.h"
#include "src/model/admin.h"


class Model : public QObject
{
    Q_OBJECT
public:
    Model(QObject *parent = nullptr);
    ~Model();
signals:
    void viewHandler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
    void controllerHandler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
    void gazePoint(double inX,double inY);

    // модернизация:

private:

    EyeTracker *_eyeTracker;
    TestsController *_testsController;
    Admin *_admin;
    QObject *_userController;

    void init();

    QFile *_file;
    QTextStream *_textFileStream;
    void openFile(QString fileName);
    void closeFile();
    void setStreamForTracking();
    ResponseAnswer_ENUM gazePointDestination;
};

#endif // MODEL_H
