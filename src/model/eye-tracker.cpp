#include "eye-tracker.h"

EyeTracker::EyeTracker() : QObject(new QObject)
{
    calibrate_done = false;
    calibrate_need = true;
    eye_tracker = nullptr;
    tracking_active = false;

    prevGazePoint.xValue = -10;
    prevGazePoint.yValue = -10;
    THRESHOLD = 10.0;
    outputStream = nullptr;


}

EyeTracker::~EyeTracker()
{
    //if(eyeTracker) // Резлизовать!!!

}

int EyeTracker::init()
{
#ifdef NO_EYE_TRACKER_TEST
    return 0;
#endif
    // TODO: Необходимо дописать и сделать: 1) после поиска всех устройств предлагать пользователю выбрать, какое использовать 2) после найденных устройст
    TobiiResearchEyeTrackers *findedTrackers;
    TobiiResearchStatus response = tobii_research_find_all_eyetrackers(&findedTrackers);
    qDebug() << (int)response << "\n";
    if (response) { // != TOBII_RESEARCH_STATUS_OK
        qDebug() << "\n" << "Error in tobii_research_find_all_eyetrackers: " << response << "\n";
        return 1;
    }
    eye_tracker = findedTrackers->eyetrackers[0];
    return 0;
}

int EyeTracker::calibrate()
{
    QJsonObject jsonPoint;
    jsonPoint["x"]=0.0f;
    jsonPoint["y"]=0.0f;
    jsonPoint["time"] = 1000;


#ifdef NO_EYE_TRACKER_TEST
    {
        emit sendSignal(EYE_TRACKER_SUCESSFULL_CALIBRATION_START);
        #define NUM_OF_POINTS  9U
        TobiiResearchNormalizedPoint2D points_to_calibrate[NUM_OF_POINTS] = \
        { {0.5f, 0.5f}, { 0.6f, 0.6f }, { 0.6f, 0.4f }, { 0.4f, 0.6f }, { 0.4f, 0.4f }, { 0.05f, 0.05f }, { 0.05f, 0.95f }, { 0.95f, 0.05f }, { 0.95f, 0.95f }};
        size_t i = 0;
        for (; i < 4; i++) {
            TobiiResearchNormalizedPoint2D* point = &points_to_calibrate[i];
            jsonPoint["x"]=point->x;
            jsonPoint["y"]=point->y;
            emit sendSignal(EYE_TRACKER_POINT_TO_CALIBRATE, JSONtoStr(jsonPoint)); // QString(QJsonDocument(jsonPoint).toJson(QJsonDocument::Compact))
            QThread::msleep(jsonPoint["time"].toInt()+500);
        }
        emit sendSignal(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION);
        QThread::msleep(5000);
        emit sendSignal(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED);
        emit sendSignal(EYE_TRACKER_LEAVE_CALIBRATION_MODE);
        return 0;
    }
#endif

    TobiiResearchStatus status = tobii_research_screen_based_calibration_enter_calibration_mode(eye_tracker);
    if (status == TOBII_RESEARCH_STATUS_OK)
        emit sendSignal(EYE_TRACKER_SUCESSFULL_CALIBRATION_START); // means successful start of calibration process
    else {
        emit sendSignal(EYE_TRACKER_FAILED_CALIBRATION); // means "can't start calibration mode"
        return 0;
    }
    /* The coordinates are normalized, i.e. (0.0, 0.0) is the upper left corner and (1.0, 1.0) is the lower right corner. */
    {
        #define NUM_OF_POINTS  9U
        srand(time(NULL));
        QList<TobiiResearchNormalizedPoint2D> listOfPoints;
        listOfPoints.append({0.5f, 0.5f}); listOfPoints.append( { 0.6f, 0.6f });
        listOfPoints.append( { 0.6f, 0.4f }); listOfPoints.append( { 0.4f, 0.6f });
        listOfPoints.append( { 0.4f, 0.4f }); listOfPoints.append( { 0.05f, 0.05f });
        listOfPoints.append( { 0.05f, 0.95f }); listOfPoints.append( { 0.95f, 0.05f });
        listOfPoints.append( { 0.95f, 0.95f });
        size_t i = 0;
        for (; i < NUM_OF_POINTS; i++) {
            int r = rand()%listOfPoints.length();
            TobiiResearchNormalizedPoint2D* point = &listOfPoints[r];
            jsonPoint["x"]=point->x;
            jsonPoint["y"]=point->y;
            emit sendSignal(EYE_TRACKER_POINT_TO_CALIBRATE, QJsonDocument(jsonPoint).toJson(QJsonDocument::Compact));
            QThread::msleep(jsonPoint["time"].toInt()+500);

            qDebug()<< "Collecting data at" << point->x<< " "<<point->y;
            if (tobii_research_screen_based_calibration_collect_data(eye_tracker, point->x, point->y) != TOBII_RESEARCH_STATUS_OK) {
                /* Try again if it didn't go well the first time. */
                /* Not all eye tracker models will fail at this point, but instead fail on ComputeAndApply. */
                tobii_research_screen_based_calibration_collect_data(eye_tracker, point->x, point->y);
            }
            listOfPoints.removeAt(r);
        }
        qDebug()<<("Computing and applying calibration.\n");
        emit sendSignal(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION);

        TobiiResearchCalibrationResult* calibration_result = NULL;
        status = tobii_research_screen_based_calibration_compute_and_apply(eye_tracker, &calibration_result);
        emit sendSignal(EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED);
        if (status == TOBII_RESEARCH_STATUS_OK && calibration_result->status == TOBII_RESEARCH_CALIBRATION_SUCCESS) {
            qDebug() << "Compute and apply returned "<<status<<" and collected at "<<calibration_result->calibration_point_count<<" points.\n";
        }
        else {
            qDebug() << "Calibration failed!\n";
        }
        tobii_research_free_screen_based_calibration_result(calibration_result);
    }

    status = tobii_research_screen_based_calibration_leave_calibration_mode(eye_tracker);

    calibrate_need = false;
    qDebug() << QString("Left calibration mode.\n");
    emit sendSignal(EYE_TRACKER_LEAVE_CALIBRATION_MODE);
    return 0;
}

void gazeDataCallback(TobiiResearchGazeData*gaze_data, void* user_data)
{
    memcpy(user_data, gaze_data, sizeof(*gaze_data));
}

int EyeTracker::startTracking()
{
    if(!eye_tracker)
        return EYE_TRACKER_DID_NOT_INIT;
    if(calibrate_need)
        return EYE_TRACKER_NEED_CALIBRATE;
    if(tracking_active)
        return EYE_TRACKER_BUSY_WITH_TRACKING;
    tracking_active = true;

#ifdef NO_EYE_TRACKER_TEST
    {

        return 0;
    }
#endif


//    if(file)
//        file->close(), file=nullptr;
//    file
    status = tobii_research_subscribe_to_gaze_data(eye_tracker,gazeDataCallback, &lastGazeData);
    if (status != TOBII_RESEARCH_STATUS_OK)
        return EYE_TRACKER_PROBLEM_WITH_TRACKING_START;
    QPointF tmpPoint;
    while(tracking_active){
        currentGazePoint = getAvrCurrentData();
        calculate();
    }
    return 0;
}

int EyeTracker::stopTracking()
{
    if(!eye_tracker)
        return EYE_TRACKER_DID_NOT_INIT;
    if(calibrate_need)
        return EYE_TRACKER_NEED_CALIBRATE;
    if(!tracking_active)
        return EYE_TRACKER_TRACKING_PROCESS_HAVE_NOT_STARTED;

    tracking_active = false;

#ifdef NO_EYE_TRACKER_TEST
    {
        emit sendSignal(EYE_TRACKER_LEFT_TRACKING_PROCESS);
        return 0;
    }
#endif

    status = tobii_research_unsubscribe_from_gaze_data(eye_tracker, gazeDataCallback);
    emit sendSignal(EYE_TRACKER_LEFT_TRACKING_PROCESS);
    return 0;
}

GazePoint EyeTracker::getAvrCurrentData()
{
    return GazePoint( (lastGazeData.left_eye.gaze_point.position_on_display_area.x +
                   lastGazeData.right_eye.gaze_point.position_on_display_area.x) / 2,
                    (lastGazeData.left_eye.gaze_point.position_on_display_area.y +
                    lastGazeData.right_eye.gaze_point.position_on_display_area.y) / 2,
                      lastGazeData.device_time_stamp, lastGazeData.system_time_stamp);
}

void EyeTracker::printCurrentData()
{
    /// отображение данных. Если выходной поток был не задан, но данные отображаются в дебаггере
    /// было бы неплохо, если бы просто происходила запись в файл, если не задан выходной поток
    if(!outputStream){
        qDebug() << currentGazePoint.xValue <<
                    currentGazePoint.yValue;

    }
    else
        *outputStream << currentGazePoint.system_time_stamp << " " <<currentGazePoint.xValue << " " <<
                        currentGazePoint.yValue << "\n";
    emit sendGazePoint(currentGazePoint.xValue, currentGazePoint.yValue);
}

void EyeTracker::calculate()
{
    if(isnan(currentGazePoint.posF().x()) || isnan(currentGazePoint.posF().y())  ||
            currentGazePoint.posF().x() < 0 || currentGazePoint.posF().y() < 0)
        return;
    QPointF tmp = currentGazePoint.posF()-prevGazePoint.posF();
    double x = sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y());
    if(x==0) return;
qDebug() << "lenght = " << x;
    if( x > THRESHOLD)
        currentGazePoint.fixaction = false;
    else
        currentGazePoint.fixaction = true;
    printCurrentData();
    prevGazePoint = currentGazePoint;
}

void EyeTracker::setTextStream(QTextStream *inStream)
{
    outputStream = inStream;
}


