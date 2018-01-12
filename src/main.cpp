#include "main-window.h"
#include "BCIs/eye-tracker/eye-tracker.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    EyeTracker eyeTracker;
    window.show();
    QTimer::singleShot(1000, &window, SLOT(showFullScreen()));

    QObject::connect(&window, &MainWindow::eyeTrakerHandler, &eyeTracker, &EyeTracker::signalsHandler);
    QObject::connect(&eyeTracker, &EyeTracker::uiHandler,&window, &MainWindow::signalsHandler);

    return a.exec();
}
