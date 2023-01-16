
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QSurfaceFormat>
#include <iostream>
#include <QObject>
#include <QApplication>

#include "application.h"


int main(int argc, char *argv[])
{
    Application::performStartupConfiguration();

    QApplication app(argc, argv);

    Application *const map3DApp = Application::instance();
    map3DApp->initialize();




    return app.exec();
}
