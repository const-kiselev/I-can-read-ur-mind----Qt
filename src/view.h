#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include "src/additional_header.h"

#include "src/view/main-window.h"
#include "src/view/menu.h"
#include "src/view/calibration.h"
#include "src/view/test-view.h"
#include "src/view/gaze-point-test.h"
#include "src/view/admin-view.h"

#include "src/controller.h"



class View : public QObject
{
    Q_OBJECT
public:
    View(Controller *ctrl, QObject *parent = nullptr);
    void init();
    void mainWindowInit();
signals:
    void controllerHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    int setMouseTracking(QTextStream *stream); // трекинг позиции мышки в поток (файл)
    void resetMouseTracking();
private:
    Controller *_ctrl;
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
