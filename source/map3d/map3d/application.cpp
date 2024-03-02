#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSplashScreen>
#include <QSurfaceFormat>
#include <QtConcurrent/QtConcurrent>

#include "application.h"
#include "loadingPage.h"
#include "mainwindow.h"
#include "mapItem.h"
#include "networkManager.h"

Application::Application()
    : mPluginManager(new PluginManager)
{
}

Application *Application::instance()
{
    static Application app;;
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
//    initializeQmlEngine();
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
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)},
                                      {"applicationCpp", QVariant::fromValue(this)}});

    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);

    connect(mUserManager, &UserManager::signedIn, this, &Application::onLoadingPage);
   // connect(this, &Application::pageIndexChanged, this, &Application::onLoadingPage);
    connect(mUserManager, &UserManager::signedOut, this, &Application::clearMainWindow);
    //--user manger------------------------------------------
//    mUserManager = new UserManager(mServiceManager, mQmlEngine);

//    mQmlEngine->load(QUrl("qrc:/ApplicationWindow.qml"));
}

//void Application::initializeQmlEngine()
//{
////    mQmlEngine = new QQmlApplicationEngine();
//    QObject::connect(mQmlEngine,
//                     &QQmlApplicationEngine::objectCreated,
//                     this,
//                     &Application::onQmlObjectCreated,
//                     Qt::DirectConnection);
//}

//void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
//{
//    if (!obj) {
//        qDebug() << "Can not create: " << objUrl.toString();
//        QCoreApplication::exit(-1);
//        return;
//    }
//    mApplicationWindow = qobject_cast<QQuickWindow *>(obj);

//    if (!mApplicationWindow) {
//        qDebug() << "Can not create application window";
//        QCoreApplication::exit(-1);
//        return;
//    }

//}

void Application::setPageIndex(int index)
{
    mPageIndex = index;
    emit pageIndexChanged();
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
    mPluginManager->unLoadPlugins();
    delete mMainWindow;
}

void Application::onLoadingPage()
{


        qDebug()<<"onLoading";
        setPageIndex(1);
        disconnect(mServiceManager, nullptr, this, nullptr);
        connect(mPluginManager, &PluginManager::pluginMessage, mLoadingPage, &LoadingPage::addItem);
        mPluginManager->loadPlugins();

        connect(mPluginManager, &PluginManager::pluginsLoaded, mPluginManager, &PluginManager::setup);

        connect(mPluginManager, &PluginManager::setupFinished, [this](){
            setPageIndex(2);
        });

}

void Application::setQmlEngine(QQmlApplicationEngine *newQmlEngine)
{
    mQmlEngine = newQmlEngine;
}

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}

int Application::pageIndex() const
{
    return mPageIndex;
}
