
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QSurfaceFormat>
#include <iostream>
#include <QObject>

#include "application.h"


int main(int argc, char *argv[])
{
    Application *const app = Application::instance();

    return app->main(argc, argv);
}
