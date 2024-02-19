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
    //--qml--------------------------------------------------
    initializeQmlEngine();
    mPluginManager->setQmlEngine(mQmlEngine);

    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
//    mNetworkManager->start();

    mServiceManager = new ServiceManager(mNetworkManager);
    mMainWindow = new MainWindow();
    mUserManager = new UserManager(mServiceManager);
    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mLoadingPage = new LoadingPage();

    mQmlEngine->setInitialProperties({{"userManager", QVariant::fromValue(mUserManager)},
                                      {"connectionConfigCpp", QVariant::fromValue(mConnectionConfig)},
                                      {"loadingPageCpp", QVariant::fromValue(mLoadingPage)},
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)}});

    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
    connect(mUserManager, &UserManager::signedOut, this, &Application::clearMainWindow);
    //--user manger------------------------------------------
//    mUserManager = new UserManager(mServiceManager, mQmlEngine);

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

        mPluginManager->loadPlugins();
        mPluginManager->setup();
}

void Application::initializeSurfaceFormat()
{
    QSurfaceFormat fmt;
    fmt.setVersion(2, 0);
    fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);
}

void Application::clearMainWindow()
{
    qDebug() << "logout----------------";
    // delete mMainWindow;
}

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}
