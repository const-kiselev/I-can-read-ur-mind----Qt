#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{

}

void Model::handler(const QString &inString)
{
    if(inString == MODEL_INIT_ALL_GADGETS){
        _eyeTracker = new EyeTracker;
        if(_eyeTracker->init())
            viewHandler(MENU_ADD_EYE_TRACKER_ACTIONS);
    }
    else if(inString == VIEW_CALIBRATION_WIDGET_READY){

    }
}
