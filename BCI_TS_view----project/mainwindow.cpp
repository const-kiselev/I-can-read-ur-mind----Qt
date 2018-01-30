#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     QFile file("../BCIs_TS_1_demo.xml");
     if(file.open(QIODevice::ReadOnly)){
        TestView *tmpTestView = new TestView(this);
        tmpTestView->load(&file);
        file.close();
        setCentralWidget(tmpTestView);
        qDebug() << file.fileName();
     }
     else
         qDebug() << "Error with file openong";

}

MainWindow::~MainWindow()
{

}
