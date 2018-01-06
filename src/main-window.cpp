#include "main-window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedWidget = new QStackedWidget;
    QWidget *pMenu = new Menu(this);
    stackedWidget->setCurrentIndex(stackedWidget->addWidget(pMenu));

    setCentralWidget(stackedWidget);
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
}
