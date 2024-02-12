
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

    connect(mPluginManager, &PluginManager::pluginsLoaded, this, &Application::ready);
    connect(this, &Application::ready, this, &Application::createApplicationQml);

    mQmlEngine->load(QStringLiteral("qrc:///MainWindow.qml"));
    mQmlEngine->load(QStringLiteral("qrc:///ListWindow.qml"));
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

    MainWindow *mainWnd = qobject_cast<MainWindow *>(obj);
    ListWindow *listWnd = qobject_cast<ListWindow *>(obj);

    if (mainWnd) {
        qDebug() << "Load: " << objUrl.toString();
        mMainWindow = mainWnd;
        mMainWindow->initComponent();

        mPluginManager->loadPlugins();
        mPluginManager->setup();
    } else if (listWnd) {
        qDebug() << "Load: " << objUrl.toString();
        mListWindow = listWnd;
        mMainWindow->setListWindow(mListWindow);
    } else {
        qDebug() << "qml object created not found!";
    }
}

void Application::createApplicationQml()
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);

    QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
        if (status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if (status == QQmlComponent::Ready) {
            mApplicationQml = qobject_cast<QQuickWindow *>(comp->create());

            qDebug() << "application window loaded";
            qDebug() << mMainWindow;
            mApplicationQml->setProperty("mainWindow", QVariant::fromValue(mMainWindow));
        }
    });

    comp->loadUrl(QUrl("qrc:/ApplicationWindow.qml"));
}

void Application::createMainWindowQml()
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);

    QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
        if (status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if (status == QQmlComponent::Ready) {
            mMainWindow = static_cast<MainWindow *>(qobject_cast<QQuickItem *>(comp->create()));

            qDebug() << "main window loaded";
        }
    });

    comp->loadUrl(QUrl("qrc:/MainWindow.qml"));
}

void Application::show()
{
    if (mIsReady) {
        mApplicationQml->show();
    } else {
        QObject::connect(this, &Application::ready, [this]() { mApplicationQml->show(); });
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
