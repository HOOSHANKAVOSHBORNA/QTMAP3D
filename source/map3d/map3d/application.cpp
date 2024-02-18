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
    mMainWindow = new MainWindow();
    mUserManager = new UserManager(mServiceManager);
    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mLoadingPage = new LoadingPage();

    mQmlEngine->setInitialProperties({{"userManager", QVariant::fromValue(mUserManager)},
                                      {"connectionConfigCpp", QVariant::fromValue(mConnectionConfig)},
                                      {"loadingPageCpp", QVariant::fromValue(mLoadingPage)},
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)}});

    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
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
        emit ready();

        mPluginManager->loadPlugins();
        mPluginManager->setup();
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

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}
