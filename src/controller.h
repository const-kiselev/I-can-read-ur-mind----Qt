#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QFile>

#include "src/additional_header.h"
#include "src/model.h"
#include "src/view.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject *parent = nullptr);
    void setViewAndModel(View *view, Model *model){_model = model; _view = view;}

signals:
    void modelHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void viewHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void appExit();
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");

    void startMouseTrackingViaViewSDK(QTextStream *stream);
    void stopMouseTrackingViaViewSDK();
private:
    Model *_model;
    View *_view;

    QList<QFile> listOfFiles;
    QStringList listFiles;
    QDir workDir;
    int findAllTests();
    void fileController(ResponseAnswer_ENUM);
    void setWorkDir(QString);
    void createFile(QString);
    int findAllFile();
};

#endif // CONTROLLER_H
