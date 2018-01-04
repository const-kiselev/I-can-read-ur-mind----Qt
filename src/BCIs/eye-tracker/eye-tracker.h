#ifndef EYETRACKE_H
#define EYETRACKE_H


#include <QApplication>
#include <tobii_research/tobii_research.h>
#include <tobii_research/tobii_research_calibration.h>
#include <tobii_research/tobii_research_eyetracker.h>
#include <tobii_research/tobii_research_streams.h>
#include <additional_header.h>
using namespace std;

class eyeTracker: public QObject
{
Q_OBJECT
public:
    eyeTracker();
    ~eyeTracker();
    //function<int(string)> getHandler();
public slots:
    int handler(string);
private:
    int init();
    int calibrate();
    int startTrackingAsinc();
    void gaze_data(TobiiResearchEyeTracker*);


    TobiiResearchEyeTracker* eye_tracker;
    bool calibrate_done;
    bool calibrate_need;
    bool tracking_active;
};

#endif // EYETRACKE_H
