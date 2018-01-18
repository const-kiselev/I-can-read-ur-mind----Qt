#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{

}

void Controller::handler(const ResponseAnswer_ENUM cmd, const QString JSONdata)
{
    qDebug() << "Controller::handler recieved: "<< cmd;
    switch (cmd) {
    case VIEW_INIT_COMRLETED:
    {
        emit modelHandler(MODEL_INIT_ALL_GADGETS);
        emit modelHandler(MODEL_INIT);
        break;
    }
    case VIEW_CALIBRATION_WIDGET_READY:
    {
        emit modelHandler(EYE_TRACKER_START_CALIBRATION);
        break;
    }
    case CONTROLLER_START_TEST_d:
    {
        emit modelHandler(CONTROLLER_START_TEST_d, JSONdata);
        break;
    }
    default:
        break;
    }
}
