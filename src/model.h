#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include "src/additional_header.h"
#include "src/model/eye-tracker.h"
#include "src/model/tests-controller.h"

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);
    ~Model();
signals:
    void viewHandler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
    void controllerHandler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString&JSONdata = "");
private:
    EyeTracker *_eyeTracker;
    TestsController *_testsController;

    void init();
};

#endif // MODEL_H
