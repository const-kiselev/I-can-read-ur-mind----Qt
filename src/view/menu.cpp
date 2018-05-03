#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent)
{
    init();
}

Menu::~Menu()
{
}

void Menu::init()
{
    QVBoxLayout *pvbxLayout = new QVBoxLayout();
    QVBoxLayout *menuLayout = new QVBoxLayout();
    QVBoxLayout *tmpLayout = new QVBoxLayout();
    setLayout(pvbxLayout);
    menuList = new QWidget();
    QWidget *tmp = new QWidget();
    menuList->setLayout(menuLayout);
    //layout()->setAlignment(menuLayout, Qt::AlignCenter);
    menuList->resize(this->size());
    tmp->setLayout(tmpLayout);
    layout()->addWidget(tmp);
    tmp->layout()->addWidget(menuList);
    menuList->setMaximumHeight(400);
    //menuLayout->setMargin(5);
    //menuLayout->setSpacing(0);
    QLabel *img = new QLabel("<img width=\"200\" src=\":/img/menu_logo\"/>");
    //QLabel *background = new QLabel("<img height=\"500\" width=\"500\" src=\":/img/menu_logo\"/>", menuList);
    //menuList->setStyleSheet("background-image: \":/img/menu_background\" ");
    //this->setStyleSheet(" background-color: white;");
    tmp->setObjectName("tetta");
    tmp->setStyleSheet("#tetta {background-image:url(\":/img/menu_background\"); "
                        "background-position: left bottom; background-repeat:no-repeat; "
                        "}");
    menuLayout->addWidget(img);
    img->setMinimumWidth(200);
    menuLayout->setStretchFactor(img, 2);

    menuList->layout()->setAlignment(img, Qt::AlignCenter);
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
   connect(pcmd2, &QPushButton::clicked, this,
           [=](){emit sendSignal(MENU_OPEN_GAZE_POINT_TEST_WIDGET); });

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

void Menu::addWidget(QWidget *wdg)
{
    menuList->layout()->addWidget(wdg);
    menuList->layout()->setAlignment(wdg, Qt::AlignCenter);
}

void Menu::clear()
{
    if ( menuList->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item = menuList->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete menuList->layout();
    }
    menuList->setLayout(new QVBoxLayout);
}


