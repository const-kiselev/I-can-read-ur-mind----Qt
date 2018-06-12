#ifndef EYETRACKE_H
#define EYETRACKE_H

#include <QApplication>
#include "../../3rdparty/tobii_research/include/tobii_research.h"
#include "../../3rdparty/tobii_research/include/tobii_research_calibration.h"
#include "../../3rdparty/tobii_research/include/tobii_research_eyetracker.h"
#include "../../3rdparty/tobii_research/include/tobii_research_streams.h"
#include "../additional_header.h"

struct GazePoint {
    float xValue;
    float yValue;
    bool fixaction;
    int64_t 	device_time_stamp,
                system_time_stamp;

    GazePoint() : fixaction(false){}
    GazePoint(float inX, float inY) : fixaction(false), xValue(inX), yValue(inY){}
    GazePoint(float inX, float inY, int64_t  deviceTS, int64_t systemTS) : fixaction(false),
        xValue(inX), yValue(inY), device_time_stamp(deviceTS), system_time_stamp(systemTS){}
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
    int init(); //todo: полностью переделать, параметром будет указатель на стрим для передачи данных о калибровке
    int calibrate(); // todo: полностью переделать, параметром будет указатель на стрим для передачи данных о калибровке
    int startTracking(); // todo: параметром будет указатель на стрим для передачи данных о калибровке
    int stopTracking();
    void setTextStream(QTextStream *inStream); // удалить?
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void sendGazePoint(double inX, double inY); // отправка уведомления об изменении gaze point
public slots:
private:

    void getCurrentData();
    void printCurrentData();
    void calculate();
    GazePoint getAvrCurrentData();

    TobiiResearchEyeTracker* eye_tracker;
    TobiiResearchStatus status;
    TobiiResearchGazeData lastGazeData;

    GazePoint   prevGazePoint,
                currentGazePoint;
    double THRESHOLD;
    bool    calibrate_done, // todo: сделать проверку по всем используемым методам
            calibrate_need, // todo: сделать проверку по всем используемым методам
            tracking_active;// todo: сделать проверку по всем используемым методам

    QTextStream *outputStream;
    QFile *file;
};

#endif // EYETRACKE_H
