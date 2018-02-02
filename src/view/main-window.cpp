#include "main-window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);
    loadingWidgetIndexInStack = -1;
    indexOfLastAddedWidgetInStack = -1;
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
}

int MainWindow::addInViewStack(QWidget *inWidget)
{
    inWidget->resize(width(), height());
    indexOfLastAddedWidgetInStack = stackedWidget->addWidget(inWidget);
    return indexOfLastAddedWidgetInStack;
}

int MainWindow::addAndShowInViewStack(QWidget *inWidget)
{
    inWidget->resize(width(), height());
    int res = stackedWidget->addWidget(inWidget);
    indexOfLastAddedWidgetInStack = res;
    stackedWidget->setCurrentIndex(res);
    if(loadingWidgetIndexInStack!=-1) // автоматическое скрытие виджета с загрузкой
        closeLoadingWidget();
    return res;
}

int MainWindow::showWidgetFromStack(QWidget *inWidget)
{
    inWidget->resize(width(), height());
    if(loadingWidgetIndexInStack!=-1) // автоматическое скрытие виджета с загрузкой
        closeLoadingWidget();
    stackedWidget->setCurrentWidget(inWidget);
    return stackedWidget->currentIndex();
}

void MainWindow::showLastAddedWidget()
{
    if(indexOfLastAddedWidgetInStack == -1)
    {
        qDebug() << "void MainWindow::showLastAddedWidget(): There's no last addedd widget in stack";
        return;
    }
    if(loadingWidgetIndexInStack!=-1) // автоматическое скрытие виджета с загрузкой
        closeLoadingWidget();
    stackedWidget->setCurrentIndex(indexOfLastAddedWidgetInStack);
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

void MainWindow::addButton(QString buttonText, QPoint buttonPos,
                           QWidget *inWidget, bool closeAction,
                           ResponseAnswer_ENUM inCmdtoViewHandler, QString inJSONdata)
{
    QPushButton *pcmd;
    if(!inWidget)
        pcmd = new QPushButton(stackedWidget->currentWidget());
    else
        pcmd = new QPushButton(inWidget);
    pcmd->setText(buttonText);

    if(buttonPos.x() == 0 && buttonPos.y() == 0)
        pcmd->move(width()-pcmd->width()-10, height()-pcmd->height()-10);
    else
        pcmd->move(buttonPos.x(), buttonPos.y());

        connect(pcmd, &QPushButton::clicked, this,
                [=](){
            if(closeAction)
            {
                if(inWidget!=this)
                    inWidget->close();
            }
            else
                emit sendSignal(inCmdtoViewHandler, inJSONdata);
        });
        return;


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
