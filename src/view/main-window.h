#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QStackedWidget>
#include "src/additional_header.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init(){}
    int addInViewStack(QWidget*inWidget);
    int addAndShowInViewStack(QWidget*inWidget);
    int showWidgetFromStack(QWidget*inWidget);
    void showLastAddedWidget();
    int showLoadingWidget(); // send pre widget id in stackedWidget
    void closeLoadingWidget();
    void removeFromStack(QWidget*inWidget);
    // develop the metod to add button to control the test or exit from
    void addButton(QString buttonText, QPoint buttonPos = {0,0},
                   QWidget* inWidget = nullptr, bool closeAction = false,
                   ResponseAnswer_ENUM inCMD = OK_ANSWER, QString inJSONdata=""); // отображаемый текст, связанный виджет в стеке виджетов (мб nullptr), закрытие виджета, команда в handler VIEW, JSONdata
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
private:

    QStackedWidget *stackedWidget;
    int loadingWidgetIndexInStack;
    int indexOfLastAddedWidgetInStack;
};

#endif // MAINWINDOW_H
