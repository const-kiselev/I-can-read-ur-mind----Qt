#include <QApplication>

#include "src/view.h"
#include "src/model.h"
#include "src/controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View _view;
    Model _model;
    Controller _controller;

    QObject::connect(&_model, SIGNAL(viewHandler(const QString&)),
                                   &_view, SLOT(handler(const QString&)));
    QObject::connect(&_view, SIGNAL(viewHandler(const QString&)),
                                   &_controller, SLOT(handler(const QString&)));
    QObject::connect(&_controller, SIGNAL(viewHandler(const QString&)),
                                   &_model, SLOT(handler(const QString&)));

    _view.init();

    return a.exec();
}
