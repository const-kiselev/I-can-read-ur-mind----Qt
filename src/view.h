#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include "additional_header.h"

#include "view/main-window.h"
#include "view/menu.h"
#include "view/calibration.h"
#include "view/test-view.h"
#include "view/gaze-point-test.h"
#include "view/admin-view.h"

#include "controller.h"



class View : public QObject
{
    Q_OBJECT
public:
    View(QObject *parent = nullptr);
    void init();
    void mainWindowInit();
signals:
    void controllerHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    int startMouseTracking(QTextStream *stream); // трекинг позиции мышки в поток (файл)
    void resetMouseTracking();
private:
    MainWindow *_mainWindow;

    //-------- WIDGETs:
    Menu *_menu, *_testSessionMenu;
    Calibration *_eyeTrackerCalibrationWidget;
    TestView *_testView;
    GazePointTest *_gazePointTest;
    AdminView *_adminView;

private:
    QTextStream *mouseTracking_stream;

protected:
    virtual void mousePressEvent  (QMouseEvent* pe);
    virtual void mouseReleaseEvent(QMouseEvent* pe);
    virtual void mouseMoveEvent   (QMouseEvent* pe);
};

#endif // VIEW_H
