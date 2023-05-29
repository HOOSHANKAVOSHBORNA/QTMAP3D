
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
#include "mainwindow.h"
#include "defenseDataManager.h"


int main(int argc, char *argv[])
{
//    qputenv("QSG_RENDER_LOOP", "basic");
    Application::performStartupConfiguration();
//    qmlRegisterType<MapItem>("Crystal",1,0,"MapItem");
//    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
////    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
//    qmlRegisterType<LayersModel>("Crystal", 1, 0, "CLayersModel");
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/MainWindow.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//        &app, [url](QObject *obj, const QUrl &objUrl) {
//            if (!obj && url == objUrl)
//                QCoreApplication::exit(-1);
//            MainWindow *mwindow = qobject_cast<MainWindow*>(obj);
//            if(mwindow)
//                qDebug()<<mwindow->getMapItem();
//        }, Qt::QueuedConnection);
//    engine.load(url);

//    MainWindow mainWindow;
    Application *const map3DApp = Application::instance();
    map3DApp->initialize();
    //--run demo------------------------
//    QObject::connect(map3DApp, &Application::defenseDataManagerInitialized, [](DefenseDataManager *defenseDataManager) {
//        Demo* demo = new Demo(defenseDataManager);
//    });
    //---------------------------------

    if (map3DApp->isMainWindowReady()) {
        map3DApp->mainWindow()->show();
    } else {
        QObject::connect(map3DApp, &Application::mainWindowCreated, [map3DApp]() {
            map3DApp->mainWindow()->showMaximized();
        });
    }
//    mainWindow.show();
    return app.exec();
}
