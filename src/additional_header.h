#pragma once
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QtWidgets>
#include <QTimer>
#include <QList>

#define NO_ET true


QString JSONtoStr(QJsonObject inJson);
QJsonObject JSON_fromStr(QString inStr);

typedef enum {
    OTHER_SIGNAL_WITH_DATA,

    EYE_TRACKER_INIT,
    EYE_TRACKER_SUCESSFULL_INIT,
    EYE_TRACKER_FAILED_INIT,
    EYE_TRACKER_START_CALIBRATION,
    EYE_TRACKER_SUCESSFULL_CALIBRATION_START,
    EYE_TRACKER_FAILED_CALIBRATION,
    EYE_TRACKER_POINT_TO_CALIBRATE,
    EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION,
    EYE_TRACKER_COMPUTING_AND_APPLYING_CALIBRATION_COMPLETED,
    EYE_TRACKER_LEAVE_CALIBRATION_MODE,

    MENU_ADD_EYE_TRACKER_ACTIONS,
    MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET,
    MENU_START_CALIBRATION,

    MENU_ADD_TEST_d,
    MENU_OPEN_WIDGET_d,
    MENU_OPEN_TEST_d,

    CONTROLLER_ALL_GADGETS_SUCESSFULL_INITED,
    CONTROLLER_CALIBRATION_WIDGET_CLICKED_START,

    VIEW_CALIBRATION_WIDGET_READY,
    VIEW_INIT_COMRLETED,
    VIEW_TEST_VIEW_LOAD_SUCCESS,
    VIEW_TEST_VIEW_LOAD_FAILED,
    VIEW_TEST_VIEW_SHOW_SUCCESS,
    VIEW_TEST_PATH_FOR_LOAD,


    MODEL_INIT,
    MODEL_INIT_ALL_GADGETS,
    MODEL_INIT_COMRLETED,
    MODEL_START_TEST_d,
    MODEL_TESTS_CONTROLLER_SUCCESS,
    MODEL_TESTS_CONTROLLER_NO_SUCH_TEST_ID,
    MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED,
    MODEL_TESTS_CONTROLLER_LOADED,
    MODEL_TESTS_CONTROLLER_NO_LOADED_TEST,
    MODEL_TESTS_CONTROLLER_SUCCESS_TEST_START,
    MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST,
    MODEL__TEST_PATH_d,

    TESTS_CONTROLLER_LIST_WITH_TESTS_WAS_UPD,

    ERROR_IN,
    ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT,
    ERROR_MODEL_EYE_TRACKER_INIT
}ResponseAnswer_ENUM;

typedef enum{
    BCI_TYPE_EYE_TRACKER
}BCI_TYPE;

struct ViewTestElement{
    int ID;
    QString name;
    BCI_TYPE _bciType;
    QString converToJSONstr()
    {
        QJsonObject tmpJson;
        tmpJson["ID"] = ID;
        tmpJson["name"] = name;
        tmpJson["BCI_TYPE"] = _bciType;
        return JSONtoStr(tmpJson);
    }
};

enum ROI_type {rect};

struct ROI_VectorElement{
    unsigned ID;
    ROI_type type;
    bool NOTforEyeTrackerFixation;
    QVector<QPoint> points;
    QPoint position;
    QString content;
    QString style; // ToDo: посмотреть, мб в Qt есть тип связанный со стилем lable
    ROI_VectorElement():NOTforEyeTrackerFixation(false){}
};

struct TestRepresinatationData{
    QVector<ROI_VectorElement> ROIsVector;
    QRect dividedLine;
    QString testStyle;
};

Q_DECLARE_METATYPE(ResponseAnswer_ENUM)


