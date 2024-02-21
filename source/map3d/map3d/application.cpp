
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include <QDir>
#include <QQuickItem>
#include <QQmlComponent>
#include <QSplashScreen>

#include "application.h"
#include "listWindow.h"
#include "loadingPage.h"
#include "mainwindow.h"
#include "mapItem.h"
#include "networkManager.h"

Application::Application() :
    mPluginManager(new PluginManager)
{
}

Application *Application::instance()
{
    static Application app;
    return &app;
}

void Application::performStartupConfiguration()
{
    qputenv("QSG_RENDER_LOOP", "basic"); // This line is very important and can not be removed
    initializeSurfaceFormat();
    //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

void Application::initialize()
{
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");

    //--qml--------------------------------------------------
    initializeQmlEngine();
    mPluginManager->setQmlEngine(mQmlEngine);

    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
//    mNetworkManager->start();

    mServiceManager = new ServiceManager(mNetworkManager);
    //--create models----------------------------------------
    mMainWindow = new MainWindow();
    mMainWindow->initComponent();
    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);

    mUserManager = new UserManager(mServiceManager);
    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mLoadingPage = new LoadingPage();


    mQmlEngine->setInitialProperties({{"userManager", QVariant::fromValue(mUserManager)},
                                      {"connectionConfigCpp", QVariant::fromValue(mConnectionConfig)},
                                      {"loadingPageCpp", QVariant::fromValue(mLoadingPage)},
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)}});

    connect(mUserManager, &UserManager::signedOut, this, &Application::clearMainWindow);
    //--user manger------------------------------------------
//    mUserManager = new UserManager(mServiceManager, mQmlEngine);

    //    connect(mPluginManager, &PluginManager::pluginsLoaded, this, &Application::ready);
    //    connect(this, &Application::ready, this, &Application::createApplicationQml);
    //createApplicationQml();
    //    mQmlEngine->load(QStringLiteral("qrc:///MainWindow.qml"));
    //    mQmlEngine->load(QStringLiteral("qrc:///ListWindow.qml"));
    mQmlEngine->load(QUrl("qrc:/ApplicationWindow.qml"));
}

void Application::initializeQmlEngine()
{
    mQmlEngine = new QQmlApplicationEngine();
    QObject::connect(mQmlEngine,
                     &QQmlApplicationEngine::objectCreated,
                     this,
                     &Application::onQmlObjectCreated,
                     Qt::DirectConnection);
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
        if (!obj) {
            qDebug() << "Can not create: " << objUrl.toString();
            QCoreApplication::exit(-1);
            return;
        }
        mApplicationWindow = qobject_cast<QQuickWindow *>(obj);

        if(!mApplicationWindow){
            qDebug() << "Can not create application window";
            QCoreApplication::exit(-1);
            return;
        }
        emit ready();

        mPluginManager->loadPlugins();
        mPluginManager->setup();
    //    MainWindow *mainWnd = qobject_cast<MainWindow *>(obj);
    //    ListWindow *listWnd = qobject_cast<ListWindow *>(obj);

    //    if (mainWnd) {
    //        auto mapItem = new MapControllerItem();
    //        qDebug() << "Load: " << objUrl.toString();
    //        mMainWindow = mainWnd;
    //        mMainWindow->setProperty("mapItem", QVariant::fromValue(mapItem));
    //        mMainWindow->initComponent(mapItem);


    ////        mPluginManager->loadPlugins();
    ////        mPluginManager->setup();
    //        emit ready();
    //    } else if (listWnd) {
    //        qDebug() << "Load: " << objUrl.toString();
    //        mListWindow = listWnd;
    //        mMainWindow->setListWindow(mListWindow);
    //    } else {
    //        qDebug() << "qml object created not found!";
    //    }
}

void Application::show()
{
    if (mIsReady) {
        mApplicationWindow->show();
    } else {
        QObject::connect(this, &Application::ready, [this]() { mApplicationWindow->show(); });
    }
}

void Application::initializeSurfaceFormat()
{
    QSurfaceFormat fmt;
    fmt.setVersion(2, 0);
    fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);
}

//void Application::onUICreated()
//{
//    //    mServiceManager->setMapObject(mMainWindow->getMapItem()->getMapObject());
//    // connect(mServiceManager, &ServiceManager::layerDataReceived, [&](CompositeAnnotationLayer *layer){
//    // mMainWindow->getMapItem()->getMapObject()->addLayer(layer);
//    // });
//    // connect(mServiceManager, &ServiceManager::clearMap, mMainWindow->getMapItem()->getMapObject(), &MapObject::clearParenticLayers);
//    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
//    mIsReady = true;
//    emit ready();
//}

void Application::clearMainWindow()
{
    qDebug() << "logout----------------";
    mPluginManager->unLoadPlugins();
    delete mMainWindow;
}

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}
