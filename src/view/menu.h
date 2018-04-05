#ifndef Menu_H
#define Menu_H

#include <QWidget>
#include "src/additional_header.h"

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

    void addEyeTrackerActions();
    void addItem(QString title, ResponseAnswer_ENUM respSignal, QString JsonResInStr = "");
    void addWidget(QWidget* wdg);
    void clear();
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
private:
    void init();
    QWidget *menuList;
};

#endif // Menu_H
