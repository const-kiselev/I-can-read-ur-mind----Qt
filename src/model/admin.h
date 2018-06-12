#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>
#include <QString>
#include <QQueue>
#include "../additional_header.h"

class Admin : public QObject
{
    Q_OBJECT
public:
    explicit Admin(QObject *parent = nullptr);
    QString getName() {return userName;}
    QJsonObject getListOfTests();
    bool checkHash(int password){return true;};
    void setHash(int hash){};
    void setUserName(QString name){};
    void setUserInfo(QString userInfo){};
    void addTestToQueue(int id, int position = -1){};
    void setTestList(QList<ViewTestElement> tests);
    void updateFields(QJsonObject input);
    QJsonObject getAllFields();
    bool isInited(){return inited;}
    void changeInited(){inited=!inited;}
    int getNextTestID();
signals:

public slots:
private:
    bool inited;
    QList<TestData> listOfTests;
    QString userName,
            userInfo;
    int hashToStop;
    int currentTest;
};

#endif // ADMIN_H
