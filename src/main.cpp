#include <QApplication>

#include "src/controller.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    Controller _controller;

    return a.exec();
}
