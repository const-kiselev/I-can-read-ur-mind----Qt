#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QThread>
#include "src/additional_header.h"
#include "src/model/eye-tracker.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void viewHandler(const QString&);
public slots:
    void handler(const QString &inString);
private:
    EyeTracker *_eyeTracker;
    ModelThread _thread;
};

class ModelThread: QThread
{
    Q_OBJECT
public:
    ModelTrread(){}
    void run()
    {
        exec();
    }
}

#endif // MODEL_H
