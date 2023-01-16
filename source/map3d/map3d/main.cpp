
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QSurfaceFormat>
#include <iostream>
#include <QObject>
#include <QApplication>
#include <QDebug>

#include "application.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    Application::performStartupConfiguration();

    QApplication app(argc, argv);

    Application *const map3DApp = Application::instance();
    QObject::connect(map3DApp, &Application::mainWindowCreated, [map3DApp]() {
        map3DApp->mainWindow()->show();
    });
    map3DApp->initialize();

    return app.exec();
}
