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
}

void Application::initialize(QQmlApplicationEngine *newQmlEngine)
{
    mQmlEngine = newQmlEngine;
    //--network----------------------------------------------
    mNetworkManager = new NetworkManager();
    mServiceManager = new ServiceManager(mNetworkManager);

    mQmlEngine->setInitialProperties({{"applicationCpp", QVariant::fromValue(this)}});
    mQmlEngine->rootContext()->setContextProperty("mapControllerCpp", QVariant::fromValue(MapControllerItem::instance()));

    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mUserManager = UserManager::instance();
    mUserManager->initialize(mServiceManager);
    emit userManagerChanged();

    connect(mUserManager, &UserManager::signedIn, this, &Application::onLoadingPage);
    connect(mUserManager, &UserManager::signedOut, this, &Application::onLogoutUser);
}

void Application::setPageIndex(int index)
{
    mPageIndex = index;
    emit pageIndexChanged();
}

UserManager *Application::userManager()
{
    return mUserManager;
}

ConnectionConfiguration *Application::connectionConfigCpp()
{
    return mConnectionConfig;
}

LoadingPage *Application::loadingPageCpp()
{
    return mLoadingPage;
}

MainWindow *Application::mainPageCpp()
{
    return mMainWindow;
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
    mLoadingPage = new LoadingPage();
    emit loadingPageCppChanged();

    mMainWindow = new MainWindow(mUserManager);
    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
    emit mainPageCppChanged();

    mPluginManager = new PluginManager;
    connect(mPluginManager, &PluginManager::pluginMessage, mLoadingPage, &LoadingPage::addItem);
    connect(mPluginManager, &PluginManager::setupFinished,this , [this](){
        setPageIndex(2);
        ReadyForData ready;
        ready.message = "Ready";
        mServiceManager->sendReady(ready);
    });

    mLoadingPage->setPluginsCount(mPluginManager->pluginFileNameList().count());
    setPageIndex(1);
    mPluginManager->loadPlugins();
}

void Application::onLogoutUser()
{
    mUserManager->logOut();
    qDebug() << "logout----------------";
    setPageIndex(0);
    if (mLoadingPage) {
        delete mLoadingPage;
        mLoadingPage = nullptr;
    }
    if (mPluginManager) {
        delete mPluginManager;
        mPluginManager = nullptr;
    }
    if (mMainWindow) {
        delete mMainWindow;
        mMainWindow = nullptr;
    }
}
