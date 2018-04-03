#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include "src/additional_header.h"

class AdminView : public QWidget
{
    Q_OBJECT
public:
    explicit AdminView(QWidget *parent = nullptr);
    void AddData(const QString JSONdata);
signals:
    void sendSignal(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
private:
    QLineEdit   *userName;
    QTextEdit   *userInfo;
    QListWidget *listOfTests;
};

#endif // ADMINVIEW_H
