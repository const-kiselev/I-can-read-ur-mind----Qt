#include "eye-tracker.h"

EyeTracker::EyeTracker() : QObject(new QObject)
{
    calibrate_done = false;
    calibrate_need = true;
    eye_tracker = nullptr;
    tracking_active = false;
}

EyeTracker::~EyeTracker()
{
    //if(eyeTracker) // Резлизовать!!!

}

int EyeTracker::init()
{
    if (NO_ET) {
        return 0;
    }
    // Задачи для реализации:
    // Необходимо дописать и сделать:
    //	- после поиска всех устройств предлагать пользователю выбрать, какое использовать
    //  - после найденных устройст
    TobiiResearchEyeTrackers *findedTrackers;
    TobiiResearchStatus response = tobii_research_find_all_eyetrackers(&findedTrackers);
    qDebug() << (int)response << "\n";
    if (response) { // != TOBII_RESEARCH_STATUS_OK
        qDebug() << "\n" << "Error in tobii_research_find_all_eyetrackers: " << response << "\n";
        return 1;
    }
    //tobii_research_get_serial_number((findedTrackers->eyetrackers[0]), &serial_number);
    //cout << serial_number << endl;
    eye_tracker = findedTrackers->eyetrackers[0];
    return 0;
}

int EyeTracker::calibrate()
{
    QJsonObject jsonPoint;
    jsonPoint["x"]=0.0f;
    jsonPoint["y"]=0.0f;
    jsonPoint["time"] = 1000;

    if (NO_ET) {
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

    TobiiResearchStatus status = tobii_research_screen_based_calibration_enter_calibration_mode(eye_tracker);
    if (status == TOBII_RESEARCH_STATUS_OK)
        emit sendSignal(EYE_TRACKER_SUCESSFULL_CALIBRATION_START); // means successful start of calibration process
    else {
        emit sendSignal(EYE_TRACKER_FAILED_CALIBRATION); // means "can't start calibration mode"
        return 0;
    }
    /* Define the points on screen we should calibrate at. */
    /* The coordinates are normalized, i.e. (0.0, 0.0) is the upper left corner and (1.0, 1.0) is the lower right corner. */
    {
        #define NUM_OF_POINTS  9U
        TobiiResearchNormalizedPoint2D points_to_calibrate[NUM_OF_POINTS] = \
        { {0.5f, 0.5f}, { 0.6f, 0.6f }, { 0.6f, 0.4f }, { 0.4f, 0.6f }, { 0.4f, 0.4f }, { 0.05f, 0.05f }, { 0.05f, 0.95f }, { 0.95f, 0.05f }, { 0.95f, 0.95f }};
        size_t i = 0;
        for (; i < NUM_OF_POINTS; i++) {
            TobiiResearchNormalizedPoint2D* point = &points_to_calibrate[i];
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
        /* Free calibration result when done using it */
        tobii_research_free_screen_based_calibration_result(calibration_result);
        /* Analyze the data and maybe remove points that weren't good. */
//        TobiiResearchNormalizedPoint2D* recalibrate_point = &points_to_calibrate[1];
//        qDebug() << "Removing calibration point at ("<<recalibrate_point->x<<","<<recalibrate_point->y<<").\n";
//        status = tobii_research_screen_based_calibration_discard_data(eye_tracker, recalibrate_point->x, recalibrate_point->y);
//        /* Redo collection at the discarded point */

//        tobii_research_screen_based_calibration_collect_data(eye_tracker, recalibrate_point->x, recalibrate_point->y);
//        /* Compute and apply again. */
//        qDebug() << ("Computing and applying calibration.\n");
//        status = tobii_research_screen_based_calibration_compute_and_apply(eye_tracker, &calibration_result);
//        if (status == TOBII_RESEARCH_STATUS_OK && calibration_result->status == TOBII_RESEARCH_CALIBRATION_SUCCESS) {
//            cout << ("Compute and apply returned %i and collected at %zu points.\n", status, calibration_result->calibration_point_count);
//        }
//        else {
//            cout << ("Calibration failed!\n");
//        }
        /* Free calibration result when done using it */
        //tobii_research_free_screen_based_calibration_result(calibration_result);
        /* See that you're happy with the result. */
    }
    /* The calibration is done. Leave calibration mode. */
    status = tobii_research_screen_based_calibration_leave_calibration_mode(eye_tracker);
    //if(status);
    qDebug() << QString("Left calibration mode.\n");
    emit sendSignal(EYE_TRACKER_LEAVE_CALIBRATION_MODE);
    return 0;
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

    status = tobii_research_subscribe_to_gaze_data(eye_tracker, gazeDataCallback, &gaze_data);
    if (status != TOBII_RESEARCH_STATUS_OK)
        return EYE_TRACKER_PROBLEM_WITH_TRACKING_START;

    QPointF tmpPoint;
    while(tracking_active){
        //gaze_data.left_eye.gaze_point.position_on_display_area.x + gaze_data.right_eye.gaze_point.position_on_display_area.x) / 2;
        //gaze_data.left_eye.gaze_point.position_on_display_area.y + gaze_data.right_eye.gaze_point.position_on_display_area.y) / 2;

        //boost::this_thread::sleep_for(boost::chrono::milliseconds(360));
        tmpPoint = getAvrCurrentData();
        emit sendGazePoint(tmpPoint.x(), tmpPoint.y());
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
    status = tobii_research_unsubscribe_from_gaze_data(eye_tracker, gazeDataCallback);
    emit sendSignal(EYE_TRACKER_LEFT_TRACKING_PROCESS);
    return 0;
}

QPointF EyeTracker::getAvrCurrentData()
{
    return QPointF( (lastGazeData.left_eye.gaze_point.position_on_display_area.x +
                   lastGazeData.right_eye.gaze_point.position_on_display_area.x) / 2,
                    (lastGazeData.left_eye.gaze_point.position_on_display_area.y +
                    lastGazeData.right_eye.gaze_point.position_on_display_area.y) / 2);
}

void EyeTracker::gazeDataCallback(TobiiResearchGazeData*, void* user_data)
{
    memcpy(user_data, gaze_data, sizeof(*gaze_data));
}
