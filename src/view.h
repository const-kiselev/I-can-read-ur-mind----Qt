#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include "src/additional_header.h"


#include "src/view/main-window.h"
#include "src/view/menu.h"
#include "src/view/calibration.h"
#include "src/view/test-view.h"


class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    void init();
    void mainWindowInit();
signals:
    void controllerHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
private:
    MainWindow *_mainWindow;

    //-------- WIDGETs:
    Menu *_menu;
    Calibration *_eyeTrackerCalibrationWidget;
    TestView *_testView;
};

#endif // VIEW_H
