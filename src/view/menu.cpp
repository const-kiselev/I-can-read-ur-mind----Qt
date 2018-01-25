#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent)
{
    init();


    setStyleSheet("background-color:white;");
    setStyleSheet("QPushButton{}");
}

Menu::~Menu()
{
}

void Menu::init()
{
    QVBoxLayout *pvbxLayout = new QVBoxLayout;
    QVBoxLayout *menuLayout = new QVBoxLayout;
    setLayout(pvbxLayout);
    menuList = new QWidget;
    menuList->setLayout(menuLayout);
    layout()->setAlignment(menuLayout, Qt::AlignCenter);
    layout()->addWidget(menuList);
    menuList->setMaximumHeight(300);
    //menuLayout->setMargin(5);
    //menuLayout->setSpacing(0);
    QLabel *img = new QLabel("<img src=\":/img/itmo_logo_full_blue\"/>");
    menuLayout->addWidget(img);
    img->setMinimumWidth(200);
    menuLayout->setStretchFactor(img, 5);
    menuList->layout()->setAlignment(img, Qt::AlignTop|Qt::AlignHCenter);

    QPushButton *pcmd1 = new QPushButton("Выйти");
    pcmd1->setMinimumWidth(300);
    menuList->layout()->addWidget(pcmd1);
    menuList->layout()->setAlignment(pcmd1, Qt::AlignCenter);
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(APP_EXIT); });

}

void Menu::addEyeTrackerActions()
{
    QPushButton *pcmd1 = new QPushButton("Калибровка");
    QPushButton *pcmd2 = new QPushButton("Проверка движения");
    pcmd1->setMinimumWidth(300);
    pcmd2->setMinimumWidth(300);
    menuList->layout()->addWidget(pcmd1);
    menuList->layout()->setAlignment(pcmd1, Qt::AlignCenter);
    menuList->layout()->addWidget(pcmd2);
    menuList->layout()->setAlignment(pcmd2, Qt::AlignCenter);


   connect(pcmd1, &QPushButton::clicked, this,
           [=](){emit sendSignal(MENU_OPEN_EYE_TRACKER_CALIBRATION_WIDGET); });

}

void Menu::addItem(QString title, ResponseAnswer_ENUM respSignal, QString JsonResInStr)
{
    QPushButton *pcmd1 = new QPushButton(title);
    pcmd1->setMinimumWidth(300);
    menuList->layout()->addWidget(pcmd1);
    menuList->layout()->setAlignment(pcmd1, Qt::AlignCenter);

    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(respSignal, JsonResInStr); });
}


