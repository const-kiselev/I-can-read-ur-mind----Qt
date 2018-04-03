#include "admin.h"

Admin::Admin(QObject *parent) : QObject(parent)
{
    userName = userInfo = "Didn't set";
    hashToStop = 0;
}

void Admin::setTestList(QList<ViewTestElement> tests)
{
    TestData tmp;
    foreach (ViewTestElement test, tests) {
        tmp.setData(test.ID, test.name, "nothing to say");
        listOfTests.append(tmp);
    }
}

QJsonObject Admin::getAllFields()
{
    QJsonObject jsonObj;
    jsonObj["userName"] = this->userName;
    jsonObj["userInfo"] = this->userInfo;
    jsonObj["allTests"] = JSONtoStr(getListOfTests());
    return jsonObj;
}


QJsonObject Admin::getListOfTests()
{
    QJsonObject jsonObj, tmpJsonObj;
    int i=0;
    foreach (TestData test, this->listOfTests) {
        tmpJsonObj["ID"] = test.ID;
        tmpJsonObj["name"] = test.name;
        tmpJsonObj["shortInfo"] = test.shortInfo;
        tmpJsonObj["active"] = test.active;
        jsonObj[i] = JSONtoStr(tmpJsonObj);
    }
    return jsonObj;
}
