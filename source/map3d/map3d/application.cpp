
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

Application::Application() :
    mPluginManager(new PluginManager)
{
}

Application *Application::instance()
{
    static Application app;
    return &app;
}

int Application::main(int argc, char **argv)
{
    qputenv("QSG_RENDER_LOOP", "basic"); // This line is very important and can not be removed

    initializeSurfaceFormat();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    initializeNetworkManager();

    registerTypes();
    initializeQmlEngine();

    mPluginManager->loadPlugins();
    QObject::connect(this, &Application::mainWindowCreated,
                     this, &Application::onMainWindowCreated,
                     Qt::DirectConnection);

    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);
    createMainWindow();
    createListWindow();

    return app.exec();
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


void Application::initializeNetworkManager()
{
    mNetworkManager = new NetworkManager;
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if (!obj && mMainWindowUrl == objUrl)
        QCoreApplication::exit(-1);
    MainWindow *mainWnd = qobject_cast<MainWindow*>(obj);
    if (mainWnd && !mMainWindow) {
        mMainWindow = mainWnd;
        emit mainWindowCreated();
    }

    ListWindow *listWnd = qobject_cast<ListWindow*>(obj);
    if (listWnd && !mListWindow) {
        mListWindow = listWnd;
        emit listWindowCreated();
    }

}

void Application::onMainWindowCreated()
{
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

   QQmlComponent *component = new QQmlComponent(mQmlEngine);

    QObject::connect(component, &QQmlComponent::statusChanged, [component, this](){
        if (component->status() == QQmlComponent::Status::Ready) {

            QQuickItem *item = reinterpret_cast<QQuickItem*>(component->create());
            this->mQmlEngine->setObjectOwnership(item, QQmlEngine::ObjectOwnership::JavaScriptOwnership);

            QMetaObject::invokeMethod(this->mMainWindow,
                                      "addItemToMainWindow",
                                      Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)));

        }
    });

    component->setData(
R"(
    import QtQuick 2.13

    Rectangle {
        x: 100
        y: 100
        width: 100
        height: 100
        color: 'red'

    }
)", QUrl());


}

void Application::setup()
{
    mPluginManager->performPluginsSetup(mMainWindow->mapController());
}

