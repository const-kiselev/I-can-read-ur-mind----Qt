#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QStackedWidget>
#include "widgets/menu/menu.h"
#include "additional_header.h"
#include "src/widgets/eyeTracker/calibration/calibration.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
signals:
    void eyeTrakerHandler(const QString&);
    void menuHandler(const QString&);
    void calibHandler(const QString&);
    void stdHandler(const QString&);
public slots:
    int signalsHandler(const QString&);

private:

    //Calibration *mainCalib;
    QStackedWidget *stackedWidget;
    QWidget *pMenu,
            *pMainCalib;


    int startInitProcess();
    int breakInitProcess();
};

#endif // MAINWINDOW_H
