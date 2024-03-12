
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
#include "logManager.h"

int main(int argc, char *argv[])
{
//    LogManager logManager;
//    qInstallMessageHandler(LogManager::messageHandler);

    //    qputenv("QSG_RENDER_LOOP", "basic");
    Application::performStartupConfiguration();
    QApplication app(argc, argv);

    QQmlApplicationEngine *qmlEngine = new QQmlApplicationEngine();
    Application *const map3DApp = Application::instance();
    map3DApp->initialize(qmlEngine);

    qmlEngine->load(QUrl("qrc:/ApplicationWindow.qml"));
    if (qmlEngine->rootObjects().isEmpty())
        return -1;

    return app.exec();
}
