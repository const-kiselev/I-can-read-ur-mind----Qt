#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void reloadXML();
    void loadNewXML();
private:

    void readXML();
     QFile *mainFile;
};

#endif // MAINWINDOW_H
