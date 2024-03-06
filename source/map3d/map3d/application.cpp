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

void Application::initialize(QQmlApplicationEngine *newQmlEngine)
{
    mQmlEngine = newQmlEngine;
    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
    mServiceManager = new ServiceManager(mNetworkManager);

    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mUserManager = new UserManager(mServiceManager);
    mLoadingPage = new LoadingPage();
    mMainWindow = new MainWindow();

    mQmlEngine->setInitialProperties({{"userManager", QVariant::fromValue(mUserManager)},
                                      {"connectionConfigCpp", QVariant::fromValue(mConnectionConfig)},
                                      {"loadingPageCpp", QVariant::fromValue(mLoadingPage)},
                                      {"mainPageCpp", QVariant::fromValue(mMainWindow)},
                                      {"applicationCpp", QVariant::fromValue(this)}});

    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);

    mPluginManager = new PluginManager;
    connect(mUserManager, &UserManager::signedIn, this, &Application::onLoadingPage);
    connect(mUserManager, &UserManager::signedIn, [this](){
        mMainWindow->getLayerManager()->userSignedIn(mUserManager);
    });
    connect(mPluginManager, &PluginManager::plugunCount, mLoadingPage, &LoadingPage::setPluginCounter);
    connect(mPluginManager, &PluginManager::pluginMessage, mLoadingPage, &LoadingPage::addItem);
    connect(mPluginManager, &PluginManager::setupFinished,this , [this](){
        setPageIndex(2);
    });
    connect(mUserManager, &UserManager::signedOut, this, &Application::clearMainWindow);
}

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

void Application::onLoadingPage()
{
        setPageIndex(1);
        mPluginManager->loadPlugins();
}

void Application::clearMainWindow()
{
    qDebug() << "logout----------------";
    setPageIndex(0);
    mPluginManager->unLoadPlugins();
    mMainWindow->clearData();
}
