
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
#include "defenseDataManager.h"


int main(int argc, char *argv[])
{
    Application::performStartupConfiguration();

    QApplication app(argc, argv);

    Application *const map3DApp = Application::instance();
    map3DApp->initialize();
    //--run demo------------------------
    Demo demo(Application::instance()->defenseDataManager());
    //---------------------------------

    if (map3DApp->isMainWindowReady()) {
        map3DApp->mainWindow()->show();
    } else {
        QObject::connect(map3DApp, &Application::mainWindowCreated, [map3DApp]() {
            map3DApp->mainWindow()->showMaximized();
        });
    }

    return app.exec();
}
