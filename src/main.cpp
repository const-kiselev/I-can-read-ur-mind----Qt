#include <QApplication>

#include "view.h"
#include "model.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    Controller _controller;
    View _view;
    Model _model;

    QObject::connect(&_model, SIGNAL(viewHandler(const ResponseAnswer_ENUM, const QString)),
                                       &_view, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    QObject::connect(&_model, SIGNAL(controllerHandler(const ResponseAnswer_ENUM, const QString)),
                                   &_controller, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    QObject::connect(&_view, SIGNAL(controllerHandler(const ResponseAnswer_ENUM, const QString)),
                                   &_controller, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    QObject::connect(&_controller, SIGNAL(modelHandler(const ResponseAnswer_ENUM, const QString)),
                                   &_model, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    QObject::connect(&_controller, SIGNAL(viewHandler(const ResponseAnswer_ENUM, const QString)),
                                   &_view, SLOT(handler(const ResponseAnswer_ENUM, const QString)));
    QObject::connect(&_controller, SIGNAL(appExit()), &a, SLOT(quit()));

    _view.init();

    return a.exec();
}
