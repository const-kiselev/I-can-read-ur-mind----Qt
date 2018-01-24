#ifndef EYETRACKE_H
#define EYETRACKE_H

#define NO_EYE_TRACKER_TEST


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
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void sendGazePoint(double inX, double inY); // отправка уведомления об изменении gaze point
public slots:
private:

    void getAvrCurrentData();
    void getCurrentData();

    TobiiResearchEyeTracker* eye_tracker;
    TobiiResearchStatus status;
    TobiiResearchGazeData lastGazeData;

    GazePoint prevGazePoint, currentGazePoint;
    bool calibrate_done;
    bool calibrate_need;
    bool tracking_active;

    // DATA methods:

#ifdef NO_EYE_TRACKER_TEST
    ///
    /// тестирование с помощью мышки
    ///
    virtual void MainWindow::mousePressEvent(QMouseEvent *event);
    virtual void MainWindow::mouseReleaseEvent(QMouseEvent *);
    virtual void MainWindow::mouseMoveEvent(QMouseEvent *event);
#endif


};

#endif // EYETRACKE_H



// Принцип работы класса:
//
