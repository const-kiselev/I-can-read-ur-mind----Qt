#ifndef Menu_H
#define Menu_H

#include <QWidget>
#include "src/additional_header.h"

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent);
    ~Menu();

    void addEyeTrackerActions();
signals:
    void sendSignal(const QString&);
private:
};

#endif // Menu_H
