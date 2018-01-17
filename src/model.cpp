#include "model.h"



Model::Model(QObject *parent) : QObject(parent)
{
}

Model::~Model()
{
}

void Model::handler(const ResponseAnswer_ENUM cmd, const QString&JSONdata)
{
    qDebug() << "Model::handler recieved: "<< cmd;
    switch (cmd) {
    case MODEL_INIT_ALL_GADGETS:
        _eyeTracker = new EyeTracker;
        qRegisterMetaType<ResponseAnswer_ENUM>();
        connect(_eyeTracker, SIGNAL(sendSignal(const ResponseAnswer_ENUM, const QString)),
                this, SLOT(handler(const ResponseAnswer_ENUM, const QString)),
                Qt::QueuedConnection);
        if(!_eyeTracker->init())
            emit viewHandler(EYE_TRACKER_SUCESSFULL_INIT);
        break;
    case EYE_TRACKER_START_CALIBRATION:
        QtConcurrent::run(_eyeTracker, &EyeTracker::calibrate);
        break;
    case EYE_TRACKER_POINT_TO_CALIBRATE:
        emit viewHandler(EYE_TRACKER_POINT_TO_CALIBRATE, JSONdata);
        break;
    case EYE_TRACKER_LEAVE_CALIBRATION_MODE:
        emit viewHandler(EYE_TRACKER_LEAVE_CALIBRATION_MODE);
        break;
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION:
        emit viewHandler(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION);
        break;
    case EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED:
        emit viewHandler(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED);
        break;
    default:
        break;
    }

}
