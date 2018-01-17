#ifndef EYETRACKE_H
#define EYETRACKE_H


#include <QApplication>
#include <tobii_research.h>
#include <tobii_research_calibration.h>
#include <tobii_research_eyetracker.h>
#include <tobii_research_streams.h>
#include "src/additional_header.h"


class EyeTracker: public QObject
{
Q_OBJECT
public:
    EyeTracker();
    ~EyeTracker();
    //function<int(string)> getHandler();
    int init();
    int calibrate();
    int startTrackingAsinc();
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
private:

    void gaze_data(TobiiResearchEyeTracker*);

    TobiiResearchEyeTracker* eye_tracker;
    TobiiResearchStatus status;
    bool calibrate_done;
    bool calibrate_need;
    bool tracking_active;

};

#endif // EYETRACKE_H
