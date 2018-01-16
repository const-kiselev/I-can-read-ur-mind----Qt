#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include "src/additional_header.h"


#include "src/view/main-window.h"
#include "src/view/menu.h"
#include "src/view/calibration.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    int init();
    void mainWindowInit();
signals:
    void controllerHandler(const QString&);
public slots:
    void handler(const QString &inString);
private:
    MainWindow *_mainWindow;

    //-------- WIDGETs:
    Menu *_menu;
    Calibration *_eyeTrackerCalibrationWidget;
};

#endif // VIEW_H
