#ifndef EYETRACKE_H
#define EYETRACKE_H

#include <QApplication>
#include <tobii_research.h>
#include <tobii_research_calibration.h>
#include <tobii_research_eyetracker.h>
#include <tobii_research_streams.h>
#include "src/additional_header.h"

struct GazePoint {
    double xValue;
    double yValue;
    bool fixaction;
    unsigned timestamp;
    GazePoint() : fixaction(false){}
    GazePoint(double inX, double inY) : fixaction(false), xValue(inX), yValue(inY){}
    QPointF posF(){
        return QPointF(xValue, yValue);
    }
};

class EyeTracker: public QObject
{
Q_OBJECT
public:
    EyeTracker();
    ~EyeTracker();
    //function<int(string)> getHandler();
    int init();
    int calibrate();
    int startTracking();
    int stopTracking();
    void setTextStream(QTextStream *inStream);
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void sendGazePoint(double inX, double inY); // отправка уведомления об изменении gaze point
public slots:
private:

    GazePoint getAvrCurrentData();
    void getCurrentData();
    void printCurrentData();
    void calculate();

    TobiiResearchEyeTracker* eye_tracker;
    TobiiResearchStatus status;
    TobiiResearchGazeData lastGazeData;

    GazePoint prevGazePoint, currentGazePoint;
    double THRESHOLD;
    bool calibrate_done;
    bool calibrate_need;
    bool tracking_active;
    QTextStream *outputStream;

    // DATA methods:




};

#endif // EYETRACKE_H



/// Принцип работы класса:
///
