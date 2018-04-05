#include "admin-view.h"

AdminView::AdminView(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *pvbxLayout = new QVBoxLayout;
    QVBoxLayout *centerLayout = new QVBoxLayout;
    setLayout(pvbxLayout);
    centerWidget = new QWidget;
    centerWidget->setLayout(centerLayout);
    layout()->addWidget(centerWidget);
    layout()->setAlignment(centerWidget, Qt::AlignCenter);
    centerWidget->setMaximumWidth(300);

    userName = new QLineEdit();
    QLabel *userNameLablel = new QLabel("&Идентификатор тестируемого:");
    userNameLablel->setBuddy(userName);
    userInfo = new QTextEdit();
    QLabel *userInfoLablel = new QLabel("&Данные о тестируемом:");
    userInfoLablel->setBuddy(userInfo);
    listOfTests = new QListWidget();
    QLabel *listOfTestsLablel = new QLabel("&Выберите тесты, которые необходимо воспроизвести\nдля данного пользователя:");
    listOfTestsLablel->setBuddy(listOfTests);
    QPushButton *pcmd1 = new QPushButton("Начать сессию тестирования");

    centerLayout->addWidget(userNameLablel);
    centerLayout->addWidget(userName);
    centerLayout->addWidget(userInfoLablel);
    centerLayout->addWidget(userInfo);
    centerLayout->addWidget(listOfTestsLablel);
    centerLayout->addWidget(listOfTests);
    centerLayout->addWidget(pcmd1);
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(VIEW_ADMIN_MODULE_PRESSED_START_TEST_SESSION); });
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
        listItem->setData(Qt::UserRole, test["ID"].toInt());
        listItem->setCheckState(test["active"].toBool()?Qt::Checked:Qt::Unchecked);
    }
    return VIEW_ADMIN_MODULE_LOADED;
}

ResponseAnswer_ENUM AdminView::draw()
{
    return OK_ANSWER;
}

QJsonObject AdminView::getFieldsContent()
{
    QJsonObject jsonObj, tmpJsonObj;
    jsonObj["userName"] = userName->text();
    jsonObj["userInfo"] = userInfo->toPlainText();
    int n = listOfTests->count();
    for(int i =0; i<n; i++){
        QListWidgetItem *tmpItem = listOfTests->item(i);
        tmpJsonObj[QString::number(tmpItem->data(Qt::UserRole).toInt())] = tmpItem->checkState() == Qt::Checked?true:false;
    }
    jsonObj["allTests"] = JSONtoStr(tmpJsonObj);
    return jsonObj;
}
