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

    mQmlEngine->setInitialProperties({{"applicationCpp", QVariant::fromValue(this)}});

    mConnectionConfig = new ConnectionConfiguration(mNetworkManager);
    mUserManager = new UserManager(mServiceManager);
    emit userManagerChanged();
    connect(mUserManager, &UserManager::signedIn, this, &Application::onLoadingPage);
    connect(mUserManager, &UserManager::signedOut, this, &Application::clearMainWindow);
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
    mPluginManager = new PluginManager;

    emit loadingPageCppChanged();
    connect(mPluginManager, &PluginManager::pluginMessage, mLoadingPage, &LoadingPage::addItem);
    connect(mPluginManager, &PluginManager::setupFinished,this , [this](){

        setPageIndex(2);
    });

    mMainWindow = new MainWindow();
    mMainWindow->getMapItem()->getMapObject()->setServiceManager(mServiceManager);
    emit mainPageCppChanged();
    setPageIndex(1);
    mPluginManager->loadPlugins();
}

void Application::clearMainWindow()
{
    qDebug() << "logout----------------";
    setPageIndex(0);
    delete mPluginManager;
    delete mMainWindow;
    delete mLoadingPage;
}
