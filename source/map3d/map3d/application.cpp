
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
    mpPluginManager(new PluginManager)
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

    mpPluginManager->loadPlugins();
    QObject::connect(this, &Application::mainWindowCreated,
                     this, &Application::onMainWindowCreated,
                     Qt::DirectConnection);

    QObject::connect(mpQmlEngine, &QQmlApplicationEngine::objectCreated,
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
    mpQmlEngine = new QQmlApplicationEngine();
}


void Application::createMainWindow()
{
    mpQmlEngine->load(mMainWindowUrl);
}

void Application::createListWindow()
{
    mpQmlEngine->load(mListWindowUrl);
}


void Application::initializeNetworkManager()
{
    mNetworkManager = new NetworkManager;
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if (!obj && mMainWindowUrl == objUrl)
        QCoreApplication::exit(-1);
    MainWindow *wnd = qobject_cast<MainWindow*>(obj);
    if (wnd && !mpMainWindow) {
        mpMainWindow = wnd;
        emit mainWindowCreated();
    }
}

void Application::onMainWindowCreated()
{
    QObject::connect(mpMainWindow, &MainWindow::sideItemCreated,
                     mpPluginManager, &PluginManager::onSideItemCreated,
                     Qt::QueuedConnection);
    QObject::connect(mpMainWindow, &MainWindow::toolboxItemCreated,
                     mpPluginManager, &PluginManager::onToolboxItemCreated,
                     Qt::DirectConnection);
    QObject::connect(mpMainWindow, &MainWindow::toolboxItemClicked,
                     mpPluginManager, &PluginManager::onToolboxItemClicked,
                     Qt::DirectConnection);
    QObject::connect(mpMainWindow, &MainWindow::toolboxItemCheckedChanged,
                     mpPluginManager, &PluginManager::onToolboxItemCheckedChanged,
                     Qt::DirectConnection);
    QObject::connect(mpMainWindow, &MainWindow::osgInitialized,
                     this, &Application::setup,
                     Qt::DirectConnection);

    QObject::connect(mpMainWindow, &MainWindow::fileItemCreated,
                     mpPluginManager, &PluginManager::onFileItemCreated,
                     Qt::DirectConnection);
    QObject::connect(mpMainWindow, &MainWindow::fileItemClicked,
                     mpPluginManager, &PluginManager::onFileItemClicked,
                     Qt::DirectConnection);

    mpPluginManager->performPluginsInitQMLDesc(mpQmlEngine);

    mpMainWindow->initializePluginsUI(mpPluginManager->pluginsInfoList());

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

   QQmlComponent *component = new QQmlComponent(mpQmlEngine);

    QObject::connect(component, &QQmlComponent::statusChanged, [component, this](){
        if (component->status() == QQmlComponent::Status::Ready) {

            QQuickItem *item = reinterpret_cast<QQuickItem*>(component->create());
            this->mpQmlEngine->setObjectOwnership(item, QQmlEngine::ObjectOwnership::JavaScriptOwnership);

            QMetaObject::invokeMethod(this->mpMainWindow,
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
    mpPluginManager->performPluginsSetup(mpMainWindow->mapController());
}

