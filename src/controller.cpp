#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{

}

void Controller::handler(const QString &inString)
{
    if(inString == VIEW_INIT_COMRLETED)
        modelHandler(MODEL_INIT_ALL_GADGETS);
    else if(inString == VIEW_CALIBRATION_WIDGET_READY)
        modelHandler(EYE_TRACKER_START_CALIBRATION);
}
