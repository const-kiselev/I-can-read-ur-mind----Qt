#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainFile = nullptr;

}

MainWindow::~MainWindow()
{

}

void MainWindow::readXML()
{
    if(centralWidget()){
        disconnect(centralWidget(), SIGNAL(reload()), this, SLOT(reloadXML()));
        disconnect(centralWidget(), SIGNAL(loadNew()), this, SLOT(loadNewXML()));
        delete centralWidget();
        setCentralWidget(nullptr);
    }
    if(mainFile->open(QIODevice::ReadOnly)){
        TestView *tmpTestView = new TestView(this);
        connect(tmpTestView, SIGNAL(reload()), this, SLOT(reloadXML()));
        connect(tmpTestView, SIGNAL(loadNew()), this, SLOT(loadNewXML()));
        tmpTestView->resize(width(), height());
        tmpTestView->load(mainFile);
        mainFile->close();
        setCentralWidget(tmpTestView);
        qDebug() << mainFile->fileName();
     }
     else
         qDebug() << "Error with file openong";
}

void MainWindow::reloadXML()
{
    readXML();
}

void MainWindow::loadNewXML()
{
    if (mainFile)
        delete mainFile;
    QString fileStr = QFileDialog::getOpenFileName(0,
                        "Выберите XML файл теста", "", "BCIs_T*.xml");
    mainFile = new QFile(fileStr);
    readXML();
}
