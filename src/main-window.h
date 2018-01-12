#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QStackedWidget>
#include "widgets/menu/menu.h"
#include "widgets/eyeTracker/calibration/calibration.h"
#include "additional_header.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void eyeTrakerHandler(const QString&);
    void menuHandler(const QString&);
public slots:
    int signalsHandler(const QString&);

private:
    QStackedWidget *stackedWidget;
    QWidget *pMenu;

    int startInitProcess();
    int breakInitProcess();
};

#endif // MAINWINDOW_H
