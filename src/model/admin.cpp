#include "admin.h"

Admin::Admin(QObject *parent) : QObject(parent)
{
    userName = userInfo = "Didn't set";
    hashToStop = 0;
    inited = false;
    currentTest = -1;
}

void Admin::setTestList(QList<ViewTestElement> tests)
{
    TestData tmp;
    foreach (ViewTestElement test, tests) {
        tmp.setData(test.ID, test.name, "nothing to say");
        listOfTests.append(tmp);
    }
}

void Admin::updateFields(QJsonObject input)
{
    this->userName = input["userName"].toString();
    this->userInfo = input["userInfo"].toString();
    QJsonObject tmpJson = JSON_fromStr(input["allTests"].toString());
    for(int i=0; i<tmpJson.length(); i++)
        listOfTests[i].active = tmpJson[QString::number(i)].toBool();

}
QJsonObject Admin::getAllFields()
{
    QJsonObject jsonObj;
    jsonObj["userName"] = this->userName;
    jsonObj["userInfo"] = this->userInfo;
    jsonObj["allTests"] = JSONtoStr(getListOfTests());
    return jsonObj;
}

int Admin::getNextTestID()
{
    int i;
    if(currentTest == -1)
        i=0;
    else
        i=currentTest+1;
    for(; i<listOfTests.count(); i++)
        if(listOfTests[i].active)
        {
            currentTest = i;
            return currentTest;
        }
    currentTest = -1;
    return -1;

}
QJsonObject Admin::getListOfTests()
{
    QJsonObject jsonObj, tmpJsonObj;
    int i=0;
    foreach (TestData test, this->listOfTests) {
        tmpJsonObj["ID"] = i;
        tmpJsonObj["name"] = test.name;
        tmpJsonObj["shortInfo"] = test.shortInfo;
        tmpJsonObj["active"] = test.active;
        jsonObj[QString::number(i)] = JSONtoStr(tmpJsonObj);
        i++;
    }
    return jsonObj;
}
