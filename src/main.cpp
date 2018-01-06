#include "main-window.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTimer::singleShot(1000, &w, SLOT(showFullScreen()));

    return a.exec();
}
