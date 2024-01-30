
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
#include "mainwindow.h"
#include "listWindow.h"
#include "mapItem.h"
#include "networkManager.h"
#include "filterManager.h"

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
    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
    qmlRegisterType<Splash>("Crystal", 1, 0, "CSplash");
    //--qml--------------------------------------------------
    initializeQmlEngine();
    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
    mNetworkManager->start();
    mServiceManager = new ServiceManager(mNetworkManager);
    //--user manger------------------------------------------
    mUserManager = new UserManager(mServiceManager, mQmlEngine);
    connect(mUserManager, &UserManager::signedIn, [this] {
        mQmlEngine->load(QStringLiteral("qrc:///SplashWindow.qml"));
    });
}

void Application::show()
{
    if (mIsReady) {
        mMainWindow->show();
    } else {
        QObject::connect(this, &Application::ready, [this]() {
            mMainWindow->show();
        });
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

void Application::initializeQmlEngine()
{
    mQmlEngine = new QQmlApplicationEngine();
    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);

    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreationFailed,[]( const QUrl &url){
        qDebug()<<"Can not create: "<< url.toString();
    });
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if(!obj){
        qDebug()<<"Can not create: "<< objUrl.toString();
        QCoreApplication::exit(-1);
        return;
    }

    MainWindow *mainWnd = qobject_cast<MainWindow*>(obj);
    ListWindow *listWnd = qobject_cast<ListWindow*>(obj);
    Splash *splash = qobject_cast<Splash*>(obj);

    if (splash) {
        qDebug()<<"Load: "<< objUrl.toString();
        mSplash = splash;
        showSplash();
    }
    if (mainWnd) {
        qDebug()<<"Load: "<< objUrl.toString();
        mMainWindow = mainWnd;
        mMainWindow->initComponent();
    }

    if (listWnd) {
        qDebug()<<"Load: "<< objUrl.toString();
        mListWindow = listWnd;
        mMainWindow->setListWindow(mListWindow);
    }
}

void Application::onUICreated()
{
//    mServiceManager->setMapObject(mMainWindow->getMapItem()->getMapObject());
    // connect(mServiceManager, &ServiceManager::layerDataReceived, [&](CompositeAnnotationLayer *layer){
            // mMainWindow->getMapItem()->getMapObject()->addLayer(layer);
        // });
    // connect(mServiceManager, &ServiceManager::clearMap, mMainWindow->getMapItem()->getMapObject(), &MapObject::clearParenticLayers);
    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
    mIsReady = true;
    emit ready();
}

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}

void Application::showSplash()
{
    mQmlEngine->load(QStringLiteral("qrc:///MainWindow.qml"));
    mQmlEngine->load(QStringLiteral("qrc:///ListWindow.qml"));
    mSplash->show();
    QTimer *splashTimer = new QTimer;
    connect(splashTimer, &QTimer::timeout, [this](){
        mPluginManager->loadPlugins();
        mPluginManager->setup();
    });
    splashTimer->setSingleShot(true);
    splashTimer->start(400);
    connect(mPluginManager, &PluginManager::pluginsLoaded, [this](){
        mSplash->hide();
        onUICreated();
    });
}
