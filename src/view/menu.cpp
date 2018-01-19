#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent)
{
}

Menu::~Menu()
{
}

void Menu::addEyeTrackerActions()
{
    QPushButton *pcmd1 = new QPushButton("Калибровка");
    QPushButton *pcmd2 = new QPushButton("Проверка движения");

    QVBoxLayout *pvbxLayout = new QVBoxLayout;
    pvbxLayout->setMargin(5);
    pvbxLayout->setSpacing(15);
    pvbxLayout->addWidget(pcmd1);
    pvbxLayout->addWidget(pcmd2);

    this->setLayout(pvbxLayout);

   connect(pcmd1, &QPushButton::clicked, this,
           [=](){emit sendSignal(MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET); });

}

void Menu::addItem(QString title, ResponseAnswer_ENUM respSignal, QString JsonResInStr)
{
    QPushButton *pcmd1 = new QPushButton(title);
    layout()->addWidget(pcmd1);

    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(respSignal, JsonResInStr); });
}
