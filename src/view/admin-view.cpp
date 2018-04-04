#include "admin-view.h"

AdminView::AdminView(QWidget *parent) : QWidget(parent)
{
    userName = new QLineEdit();
    userInfo = new QTextEdit();
    listOfTests = new QListWidget();
}

ResponseAnswer_ENUM AdminView::addData(const QString JSONdata)
{
    QJsonObject json = JSON_fromStr(JSONdata), jsonAllTests;
    QListWidgetItem *listItem;
    userName->clear();
    userInfo->clear();
    listOfTests->clear();
    userName->setText(json["userName"].toString());
    userInfo->setText(json["userInfo"].toString());
    jsonAllTests = JSON_fromStr(json["allTests"].toString());
    int n = jsonAllTests.length();
    for(int i=0; i<n; i++)
    {
        QJsonObject test = JSON_fromStr(jsonAllTests[QString::number(i)].toString());
        listItem = new QListWidgetItem(test["name"].toString(), listOfTests);
        listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
        //listItem->setCheckState(test["active"].toBool());
    }
    return VIEW_ADMIN_MODULE_LADED;
}

ResponseAnswer_ENUM AdminView::draw()
{

}
