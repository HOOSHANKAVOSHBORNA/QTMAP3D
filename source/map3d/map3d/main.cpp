
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QSurfaceFormat>
#include <iostream>
#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QQuickView>

#include "application.h"


int main(int argc, char *argv[])
{
//    qputenv("QSG_RENDER_LOOP", "basic");
    Application::performStartupConfiguration();
    QApplication app(argc, argv);

    Application *const map3DApp = Application::instance();
    //--run demo------------------------
   // QObject::connect(map3DApp, &Application::defenseDataManagerInitialized, [](DefenseDataManager *defenseDataManager) {
   //     Demo* demo = new Demo(defenseDataManager);
   // });
    //---------------------------------
    map3DApp->initialize();

    return app.exec();
}
