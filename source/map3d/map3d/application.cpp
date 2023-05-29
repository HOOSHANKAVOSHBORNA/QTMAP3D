
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include <QDir>
#include <QQuickItem>

#include "application.h"
#include "mainwindow.h"
#include "listwindow.h"
#include "layersmodel.h"

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

    //initializeNetworkManager();
    registerTypes();
    initializeQmlEngine();

    mPluginManager->loadPlugins();
    QObject::connect(this, &Application::mainWindowCreated,
                     this, &Application::onMainWindowCreated,
                     Qt::DirectConnection);
    QObject::connect(this, &Application::listWindowCreated,
                     this, &Application::onListWindowCreated,
                     Qt::DirectConnection);

    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);

    createMainWindow();
    createListWindow();

}

void Application::initializeSurfaceFormat()
{
    QSurfaceFormat fmt;
    fmt.setVersion(2, 0);
    fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);
}

void Application::registerTypes()
{
    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
    qmlRegisterType<LayersModel>("Crystal", 1, 0, "CLayersModel");
    qmlRegisterType<MapItem>("Crystal",1,0,"MapItem");
}

void Application::initializeQmlEngine()
{
    mQmlEngine = new QQmlApplicationEngine();
}


void Application::createMainWindow()
{
    mQmlEngine->load(mMainWindowUrl);
}

void Application::createListWindow()
{
    mQmlEngine->load(mListWindowUrl);
}


//void Application::initializeNetworkManager()
//{
//    mNetworkManager = new NetworkManager;
//}

void Application::initializeDefenseDataManager()
{
    //TODO set parent
//    mDefenseDataManager = new DefenseDataManager;
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if (!obj && mMainWindowUrl == objUrl)
        QCoreApplication::exit(-1);
    MainWindow *mainWnd = qobject_cast<MainWindow*>(obj);
    ListWindow *listWnd = qobject_cast<ListWindow*>(obj);






    if (mainWnd && !mMainWindow) {
        mMainWindow = mainWnd;
        emit mainWindowCreated();
    }
    if (listWnd && !mListWindow) {
        mListWindow = listWnd;
        emit listWindowCreated();
    }


}

void Application::onMainWindowCreated()
{
    mMainWindowIsReady = true;

    if (mMainWindowIsReady && mListWindowIsReady) {
        onAllWindowsCreated();
    }
}

void Application::onListWindowCreated()
{
    mListWindowIsReady = true;

    if (mMainWindowIsReady && mListWindowIsReady) {
        onAllWindowsCreated();
    }
}

void Application::onAllWindowsCreated()
{
    if (mMainWindow && mListWindow) {
        static bool bFirst = true;
        if (bFirst) {
            mMainWindow->setListWindow(mListWindow);
            bFirst = false;
        }
    }



    QObject::connect(mMainWindow, &MainWindow::sideItemCreated,
                     mPluginManager, &PluginManager::onSideItemCreated,
                     Qt::QueuedConnection);
    QObject::connect(mMainWindow, &MainWindow::toolboxItemCreated,
                     mPluginManager, &PluginManager::onToolboxItemCreated,
                     Qt::DirectConnection);
    QObject::connect(mMainWindow, &MainWindow::toolboxItemClicked,
                     mPluginManager, &PluginManager::onToolboxItemClicked,
                     Qt::DirectConnection);
    QObject::connect(mMainWindow, &MainWindow::toolboxItemCheckedChanged,
                     mPluginManager, &PluginManager::onToolboxItemCheckedChanged,
                     Qt::DirectConnection);
    QObject::connect(mMainWindow, &MainWindow::osgInitialized,
                     this, &Application::setup,
                     Qt::DirectConnection);

    QObject::connect(mMainWindow, &MainWindow::fileItemCreated,
                     mPluginManager, &PluginManager::onFileItemCreated,
                     Qt::DirectConnection);
    QObject::connect(mMainWindow, &MainWindow::fileItemClicked,
                     mPluginManager, &PluginManager::onFileItemClicked,
                     Qt::DirectConnection);

    mPluginManager->performPluginsInitQMLDesc(mQmlEngine);

    mMainWindow->initializePluginsUI(mPluginManager->pluginsInfoList());


}

void Application::setup()
{
    initializeDefenseDataManager();
//    mPluginManager->performPluginsSetup(mMainWindow->getMapItem());
    emit defenseDataManagerInitialized(mDefenseDataManager);
}

