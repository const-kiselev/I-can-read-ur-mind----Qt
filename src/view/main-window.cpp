#include "main-window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);
    loadingWidgetIndexInStack = -1;
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
}

int MainWindow::addInViewStack(QWidget *inWidget)
{
    return stackedWidget->addWidget(inWidget);
}

int MainWindow::addAndShowInViewStack(QWidget *inWidget)
{
    inWidget->resize(width(), height());
    int res = stackedWidget->addWidget(inWidget);
    stackedWidget->setCurrentIndex(res);
    if(loadingWidgetIndexInStack!=-1) // автоматическое скрытие виджета с загрузкой
        closeLoadingWidget();
    return res;
}

int MainWindow::showWidgetFromStack(QWidget *inWidget)
{
    inWidget->resize(width(), height());
    stackedWidget->setCurrentWidget(inWidget);
    return stackedWidget->currentIndex();
}

void MainWindow::closeLoadingWidget()
{
    qDebug() << "MainWindow::closeLoadingWidget()";
    if(loadingWidgetIndexInStack==-1)
        return;
    QWidget *tmpWidget = stackedWidget->widget(loadingWidgetIndexInStack);
    stackedWidget->removeWidget(tmpWidget);
    delete tmpWidget;
    loadingWidgetIndexInStack = -1;
}

void MainWindow::removeFromStack(QWidget *inWidget)
{
    stackedWidget->removeWidget(inWidget);
}

int MainWindow::showLoadingWidget()
{
    qDebug() << "MainWindow::showLoadingWidget()";
    int preIndex = stackedWidget->currentIndex();
    QWidget* tmpWid = new QWidget;
    QMovie *movie = new QMovie(":/img/spinner");
    QVBoxLayout *pvbx = new QVBoxLayout;
    QLabel *processLabel = new QLabel;
    processLabel->setMovie(movie);
     movie->start();
     pvbx->addWidget(processLabel);
     pvbx->addWidget(new QLabel("<CENTER>Обработка данных...</CENTER>"));
     pvbx->setAlignment(Qt::AlignCenter);
     tmpWid->setLayout(pvbx);
    loadingWidgetIndexInStack = stackedWidget->addWidget(tmpWid);
    stackedWidget->setCurrentWidget(tmpWid);
    return preIndex;
}
