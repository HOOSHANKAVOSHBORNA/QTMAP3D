
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
#include "mapControllerItem.h"

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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

}

void Application::initialize()
{

    //initializeNetworkManager();
    registerTypes();
    initializeQmlEngine();
    initializeDefenseDataManager();


//    QObject::connect(this, &Application::mainWindowCreated,
//                     this, &Application::onMainWindowCreated,
//                     Qt::DirectConnection);
//    QObject::connect(this, &Application::listWindowCreated,
//                     this, &Application::onListWindowCreated,
//                     Qt::DirectConnection);

    createMainWindow();
    createListWindow();

}

void Application::show()
{
    if (mUIIsReady) {
        mMainWindow->show();
    } else {
        QObject::connect(this, &Application::uiCreated, [this]() {
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

void Application::registerTypes()
{
    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
    qmlRegisterType<LayersModel>("Crystal", 1, 0, "CLayersModel");
    qmlRegisterType<MapItem>("Crystal",1,0,"MapItem");
    qmlRegisterType<MapControllerItem>("Crystal",1,0,"MapController");
}

void Application::initializeQmlEngine()
{
    mQmlEngine = new QQmlApplicationEngine();
    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);
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
//    mNetworkManager = new NetworkManager;mMainWindow
//}

void Application::initializeDefenseDataManager()
{
    //TODO set parent
    mDefenseDataManager = new DefenseDataManager;
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if (!obj && mMainWindowUrl == objUrl)
        QCoreApplication::exit(-1);
    MainWindow *mainWnd = qobject_cast<MainWindow*>(obj);
    ListWindow *listWnd = qobject_cast<ListWindow*>(obj);






    if (mainWnd && !mMainWindow) {
        mMainWindow = mainWnd;
        if(mListWindow)
            onUICreated();
//        emit mainWindowCreated();
    }
    if (listWnd && !mListWindow) {
        mListWindow = listWnd;
        if(mMainWindow)
            onUICreated();

//        emit listWindowCreated();
    }


}

void Application::onUICreated()
{
    mUIIsReady = true;
    mPluginManager->loadPlugins();
    setup();
    emit uiCreated();
}

//void Application::onMainWindowCreated()
//{
//    mMainWindowIsReady = true;

//    if (mMainWindowIsReady && mListWindowIsReady) {
//        onAllWindowsCreated();
//    }
//}

//void Application::onListWindowCreated()
//{
//    mListWindowIsReady = true;

//    if (mMainWindowIsReady && mListWindowIsReady) {
//        onAllWindowsCreated();
//    }
//}

//void Application::onAllWindowsCreated()
//{
//    if (mMainWindow && mListWindow) {
//        static bool bFirst = true;
//        if (bFirst) {
//            mMainWindow->setListWindow(mListWindow);
//            bFirst = false;
//        }
//    }



//    QObject::connect(mMainWindow, &MainWindow::sideItemCreated,
//                     mPluginManager, &PluginManager::onSideItemCreated,
//                     Qt::QueuedConnection);
//    QObject::connect(mMainWindow, &MainWindow::toolboxItemCreated,
//                     mPluginManager, &PluginManager::onToolboxItemCreated,
//                     Qt::DirectConnection);
//    QObject::connect(mMainWindow, &MainWindow::toolboxItemClicked,
//                     mPluginManager, &PluginManager::onToolboxItemClicked,
//                     Qt::DirectConnection);
//    QObject::connect(mMainWindow, &MainWindow::toolboxItemCheckedChanged,
//                     mPluginManager, &PluginManager::onToolboxItemCheckedChanged,
//                     Qt::DirectConnection);
//    QObject::connect(mMainWindow, &MainWindow::osgInitialized,
//                     this, &Application::setup,
//                     Qt::DirectConnection);

//    QObject::connect(mMainWindow, &MainWindow::fileItemCreated,
//                     mPluginManager, &PluginManager::onFileItemCreated,
//                     Qt::DirectConnection);
//    QObject::connect(mMainWindow, &MainWindow::fileItemClicked,
//                     mPluginManager, &PluginManager::onFileItemClicked,
//                     Qt::DirectConnection);

//    mPluginManager->performPluginsInitQMLDesc(mQmlEngine);

//    mMainWindow->initializePluginsUI(mPluginManager->pluginsInfoList());

//    setup();

//}

void Application::setup()
{
//    mPluginManager->performPluginsInitQMLDesc(mQmlEngine);

//    mMainWindow->initializePluginsUI(mPluginManager->pluginsInfoList());
    mPluginManager->setup();

//    mPluginManager->performPluginsSetup(mMainWindow->getMapItem());
//    emit defenseDataManagerInitialized(mDefenseDataManager);
}

