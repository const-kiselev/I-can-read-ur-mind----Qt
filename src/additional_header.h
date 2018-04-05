#pragma once
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QtWidgets>
#include <QTimer>
#include <QList>

#define NO_ET true
#define NO_EYE_TRACKER_TEST
#define ADMIN_MODULE true


QString JSONtoStr(QJsonObject inJson);
QJsonObject JSON_fromStr(QString inStr);

typedef enum {
    OK_ANSWER,
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
    EYE_TRACKER_START_TRACKING,
    EYE_TRACKER_STOP_TRACKING,
    EYE_TRACKER_BUSY_WITH_TRACKING,
    EYE_TRACKER_PROBLEM_WITH_TRACKING_START,
    EYE_TRACKER_NEED_CALIBRATE,
    EYE_TRACKER_DID_NOT_INIT,
    EYE_TRACKER_TRACKING_PROCESS_HAVE_NOT_STARTED,
    EYE_TRACKER_LEFT_TRACKING_PROCESS,
    EYE_TRACKER_GAZE_POINT,
    EYE_TRACKER_READY,
    EYE_TRACKER_STARTED_TRACKING,
    EYE_TRACKER_STOPED_TRACKING,

    MENU_ADD_EYE_TRACKER_ACTIONS,
    MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET,
    MENU_OPEN_GAZE_POINT_TEST_WIDGET,
    MENU_START_CALIBRATION,

    MENU_ADD_TEST_d,
    MENU_OPEN_WIDGET_d,
    MENU_OPEN_TEST_d,
    MENU_ADD_ADMIN_BUTTON,
    MENU_OPEN_ADMIN_MODULE,

    CONTROLLER_ALL_GADGETS_SUCESSFULL_INITED,
    CONTROLLER_CALIBRATION_WIDGET_CLICKED_START,
    CONTROLLER_ADD_NEW_TEST_FILE_WITH_DIALOG, // не реализован
    CONTROLLER_NEW_TEST_FILE_d,

    VIEW,
    VIEW_CALIBRATION_WIDGET_READY,
    VIEW_INIT_COMRLETED,
    VIEW_TEST_VIEW_LOAD_SUCCESS,
    VIEW_TEST_VIEW_LOAD_FAILED,
    VIEW_TEST_VIEW_SHOW_SUCCESS,
    VIEW_TEST_PATH_FOR_LOAD,
    VIEW_TEST_CLOSE_TEST,
    VIEW_TEST_NEXT_ANSWER_FRAME,
    VIEW_TEST_NEXT_PROBLEM,
    VIEW_TEST_PRESSED_NEXT,
    VIEW_PROBLEM_WAS_CLOSED,
    VIEW_GAZE_POINT_TEST_WIDGET_READY,
    VIEW_SHOW_ADDED_WIDGET,
    VIEW_CLOSE_GAZE_POINT_TEST,
    VIEW_ADMIN_MODULE_DATA_d,
    VIEW_ADMIN_MODULE_UPDATE_DATA,
    VIEW_ADMIN_MODULE_LOADED,
    VIEW_ADMIN_MODULE_CLOSE_WIDGET,
    VIEW_ADMIN_MODULE_UPDAT_DATA_d,
    VIEW_ADMIN_MODULE_PRESSED_START_TEST_SESSION,
    VIEW_ADMIN_MODULE_START_TEST_SESSION,
    VIEW_OPEN_TEST_SESSION_MENU,
    VIEW_TEST_SESSION_CALIBRATION,
    VIEW_TEST_SESSION_MENU_READY_FOR_TEST,
    VIEW_TEST_SESSION_MENU_CLOSE,

    VIEW_WINDOW_SIZE_d,

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
    MODEL_ADD_TEST_d,
    MODEL_TEST_WAS_ADDED_d, // в контроллере автоматически переходит на MENU_ADD_TEST_d
    MODEL_TEST_WAS_DELETED_d,
    MODEL_RAW_GAZE_POINT_notJSON_d,
    MODEL_SET_GAZE_POINT_DESTINATION_notJSON_d,
    MODEL_ADMIN_MODULE_CREATED,
    MODEL_ADMIN_MODULE_INIT,
    MODEL_ADMIN_MODULE_GET_DATA,
    MODEL_ADMIN_MODULE_DATA_d,
    MODEL_ADMIN_MODULE_UPDATE_DATA_d,
    MODEL_ADMIN_START_TEST_SESSION,
    MODEL_ADMIN_NEXT_TEST,
    MODEL_ADMIN_THERE_IS_NO_TEST,

    TESTS_CONTROLLER_LIST_WITH_TESTS_WAS_UPD,

    ERROR_IN,
    ERROR_MODEL_INIT_TESTS_CONTROLLER_INIT,
    ERROR_MODEL_EYE_TRACKER_INIT,
    APP_EXIT
}ResponseAnswer_ENUM;

typedef enum{
    BCI_TYPE_EYE_TRACKER
}BCI_TYPE;

struct ViewTestElement{
    int ID;
    QString name;
    QString converToJSONstr()
    {
        QJsonObject tmpJson;
        tmpJson["ID"] = ID;
        tmpJson["name"] = name;
        return JSONtoStr(tmpJson);
    }
};

enum ROI_type {ROI_rect};

struct ROI_VectorElement{
    unsigned ID;
    ROI_type type;
    bool NOTforEyeTrackerFixation;
    QVector<QPointF> points;
    QPointF position;
    QString content;
    QString style; // ToDo: посмотреть, мб в Qt есть тип связанный со стилем lable
    QString inputType;
    int inputID;
    bool ROI;
    ROI_VectorElement():NOTforEyeTrackerFixation(false),ROI(true){}
};

struct TestRepresinatationData{
    QVector<ROI_VectorElement> ROIsVector;
    QRect dividedLine;
    QString testStyle;
};

struct TestData{
    int ID;
    QString name;
    QString shortInfo;
    bool active;
    TestData(){}
    TestData(int ID, QString name, QString shortInfo, bool active)
    {
        this->ID = ID;
        this->name = name;
        this->shortInfo = shortInfo;
        this->active = active;
    }
    void changeActive(){
        this->active = !this->active;
    }
    void changeActive(bool active){
        this->active = active;
    }
    void setData(int ID, QString name, QString shortInfo, bool active = false)
    {
        this->ID = ID;
        this->name = name;
        this->shortInfo = shortInfo;
        this->active = active;
    }
    static QList<TestData> getListOfActiveTests(QList<TestData> *list){
        QList<TestData> res;
        foreach (TestData testData, *list)
            if(testData.active)
                res.append(testData);
        return res;
    }
    static QString toJSONstring(TestData* test){
        QJsonObject json;
        json["ID"] = test->ID;
        json["name"] = test->name;
        json["shortInfo"] = test->shortInfo;
        json["active"] = test->active;
        return JSONtoStr(json);
    }
};

Q_DECLARE_METATYPE(ResponseAnswer_ENUM)


