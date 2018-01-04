#include "eye-tracker.h"

eyeTracker::eyeTracker()
{
    calibrate_done = false;
    calibrate_need = true;
    eye_tracker = nullptr;
}

eyeTracker::~eyeTracker()
{
    //if(eyeTracker) // Резлизовать!!!

}

int eyeTracker::handler(string inStr)
{
    if(inStr == "init"){
        init();
    }
    else if(inStr == "calibrate"){
        calibrate();
    }
    else if(inStr == "subscribe"){

    }
    else if(inStr == "unsubscribe"){

    }
    return 0;
}

int eyeTracker::init()
{
    // Задачи для реализации:
    // Необходимо дописать и сделать:
    //	- после поиска всех устройств предлагать пользователю выбрать, какое использовать
    //  - после найденных устройст
    TobiiResearchEyeTrackers *findedTrackers;
    TobiiResearchStatus response = tobii_research_find_all_eyetrackers(&findedTrackers);
    cout << response << endl;
    if (response) { // != TOBII_RESEARCH_STATUS_OK
        cout << endl << "Error in tobii_research_find_all_eyetrackers: " << response << endl;
        return 1;
    }
    //tobii_research_get_serial_number((findedTrackers->eyetrackers[0]), &serial_number);
    //cout << serial_number << endl;
    eye_tracker = findedTrackers->eyetrackers[0];
    return 0;
}

int eyeTracker::calibrate()
{
    // TODO: gроверить все cout, чтобы корректно отображались
    // TODO: реализовать тестирование класса без айТрекера
//    if (NO_ET) {

//        return 0;
//    }

    //pt::ptree tmpPtree;
    TobiiResearchStatus status = tobii_research_screen_based_calibration_enter_calibration_mode(eye_tracker);

    //string tmp;
    if (status == TOBII_RESEARCH_STATUS_OK){}
        //chrome->sendMsg("10"); // means successful start of calibration process
    else {
        //chrome->sendMsg("11"); // means "can't start calibration mode"
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
            //tmpPtree.put("x", point->x);
            //tmpPtree.put("y", point->y);

            //chrome->makeActionInApp(SEND_COORDINATES_FOR_CALIBRATION, &tmpPtree);
            //tmpPtree.clear();
            //  Wait a little for user to focus.
            //boost::this_thread::sleep_for(boost::chrono::milliseconds(800));

            cout<< "Collecting data at" << point->x<< " "<<point->y;
            if (tobii_research_screen_based_calibration_collect_data(eye_tracker, point->x, point->y) != TOBII_RESEARCH_STATUS_OK) {
                /* Try again if it didn't go well the first time. */
                /* Not all eye tracker models will fail at this point, but instead fail on ComputeAndApply. */
                tobii_research_screen_based_calibration_collect_data(eye_tracker, point->x, point->y);
            }
        }
        cout<<("Computing and applying calibration.\n");
        TobiiResearchCalibrationResult* calibration_result = NULL;
        status = tobii_research_screen_based_calibration_compute_and_apply(eye_tracker, &calibration_result);
        if (status == TOBII_RESEARCH_STATUS_OK && calibration_result->status == TOBII_RESEARCH_CALIBRATION_SUCCESS) {
            cout << ("Compute and apply returned %i and collected at %zu points.\n", status, calibration_result->calibration_point_count);
        }
        else {
            cout << ("Calibration failed!\n");
        }
        /* Free calibration result when done using it */
        tobii_research_free_screen_based_calibration_result(calibration_result);
        /* Analyze the data and maybe remove points that weren't good. */
        TobiiResearchNormalizedPoint2D* recalibrate_point = &points_to_calibrate[1];
        cout << ("Removing calibration point at (%f,%f).\n", recalibrate_point->x, recalibrate_point->y);
        status = tobii_research_screen_based_calibration_discard_data(eye_tracker, recalibrate_point->x, recalibrate_point->y);
        /* Redo collection at the discarded point */

        tobii_research_screen_based_calibration_collect_data(eye_tracker, recalibrate_point->x, recalibrate_point->y);
        /* Compute and apply again. */
        cout << ("Computing and applying calibration.\n");
        status = tobii_research_screen_based_calibration_compute_and_apply(eye_tracker, &calibration_result);
        if (status == TOBII_RESEARCH_STATUS_OK && calibration_result->status == TOBII_RESEARCH_CALIBRATION_SUCCESS) {
            cout << ("Compute and apply returned %i and collected at %zu points.\n", status, calibration_result->calibration_point_count);
        }
        else {
            cout << ("Calibration failed!\n");
        }
        /* Free calibration result when done using it */
        tobii_research_free_screen_based_calibration_result(calibration_result);
        /* See that you're happy with the result. */
    }
    /* The calibration is done. Leave calibration mode. */
    status = tobii_research_screen_based_calibration_leave_calibration_mode(eye_tracker);
    if(status)
    cout << ("Left calibration mode.\n");
    //chrome->sendMsg("19"); // means "closing calibration mode"
    return 0;
}

int eyeTracker::startTrackingAsinc()
{

}

void gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data) {
    memcpy(user_data, gaze_data, sizeof(*gaze_data));
}

void eyeTracker::gaze_data(TobiiResearchEyeTracker* eyetracker) {
    //pt::ptree tmpPtree;
    TobiiResearchGazeData gaze_data;
    TobiiResearchStatus status = tobii_research_subscribe_to_gaze_data(eye_tracker, gaze_data_callback, &gaze_data);
    if (status != TOBII_RESEARCH_STATUS_OK)
        return;
    ///* Wait while some gaze data is collected. */

    for (int i = 0; i < 200; i++) {
        //tmpPtree.put("x", ((gaze_data.left_eye.gaze_point.position_on_display_area.x + gaze_data.right_eye.gaze_point.position_on_display_area.x) / 2));
        //tmpPtree.put("y", (gaze_data.left_eye.gaze_point.position_on_display_area.y + gaze_data.right_eye.gaze_point.position_on_display_area.y) / 2);
        //chrome->makeActionInApp(1, &tmpPtree);
        //tmpPtree.clear();
        //boost::this_thread::sleep_for(boost::chrono::milliseconds(360));
    }

    status = tobii_research_unsubscribe_from_gaze_data(eye_tracker, gaze_data_callback);

}


/*
function<int(string)> ET::getHandler()
{
    return bind(&ET::handler, this, placeholders::_1);
}
*/

