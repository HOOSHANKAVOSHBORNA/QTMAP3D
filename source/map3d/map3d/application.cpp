
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
#include "connectionConfiguration.h"
#include "listWindow.h"
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
    //    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
    qmlRegisterType<Splash>("Crystal", 1, 0, "CSplash");

    //--qml--------------------------------------------------
    initializeQmlEngine();
    //--create models----------------------------------------
    mMainWindow = new MainWindow();
    mMainWindow->initComponent();

    LoginPage *loginPage = new LoginPage(mServiceManager, mQmlEngine);
    ConnectionConfiguration *connectionConfiguration = new ConnectionConfiguration;
    LoadingInfo *loadingPage = new LoadingInfo();
    mQmlEngine->setInitialProperties({{"loginPageCpp", QVariant::fromValue(loginPage)},
                                      {"connectionConfigCpp", QVariant::fromValue(connectionConfiguration)},
                                      {"loadingPageCpp", QVariant::fromValue(loadingPage)},
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)}});
    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
    mNetworkManager->start();

    mServiceManager = new ServiceManager(mNetworkManager);

    //--user manger------------------------------------------
    mUserManager = new UserManager(mServiceManager, mQmlEngine);

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

    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreationFailed, [](const QUrl &url) {
        qDebug() << "Can not create: " << url.toString();
    });
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
