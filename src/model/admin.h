#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <src/additional_header.h>

class Admin : public QObject
{
    Q_OBJECT
public:
    explicit Admin(QObject *parent = nullptr);
    QString getName();
    QJsonObject getListOfTests();
    bool checkHash(int password);
    void setHash(int hash);
    void setUserName(QString name);
    void setUserInfo(QString userInfo);
    void addTestToQueue(int id, int position = -1);
    void setTestList(QList<ViewTestElement> tests);
    QJsonObject getAllFields();
signals:

public slots:
private:
    QList<TestData> listOfTests;
    QString userName,
            userInfo;
    int hashToStop;
};

#endif // ADMIN_H
