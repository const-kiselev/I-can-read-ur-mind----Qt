#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    connect(_eyeTracker, SIGNAL(finished()), &_thread, SLOT(quit()));
}

void Model::handler(const QString &inString)
{
    if(inString == MODEL_INIT_ALL_GADGETS){
        _eyeTracker = new EyeTracker;
        if(_eyeTracker->init())
            emit viewHandler(MENU_ADD_EYE_TRACKER_ACTIONS);
    }
    else if(inString == VIEW_CALIBRATION_WIDGET_READY){
        _eyeTracker->moveToThread(&_modelThread);
        _modelThread.start();
        _eyeTracker->calibrate();
    }
    else if(inString.mid(0,3)==EYE_TRACKER_POINT_TO_CALIBRATE){
        emit viewHandler(inString);
        QJsonDocument doc(QJsonDocument::fromJson(inSignal.mid(3,inSignal.length()-3).toUtf8()));
        // Get JSON object
        QJsonObject json = doc.object();
        qDebug() << inSignal.mid(2,inSignal.length()-3);
        qDebug() << json;
        // Access properties
        qDebug() << json["x"].toDouble() << " " << json["y"].toDouble();
        moveTo(json["x"].toDouble(), json["y"].toDouble(), json["time"].toInt());
    }
    }
}
