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
