
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include <QDir>

#include "application.h"
#include "mainwindow.h"

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


    registerTypes();
    initializeQmlEngine();

    mpPluginManager->loadPlugins();
    QObject::connect(this, &Application::mainWindowCreated,
                     this, &Application::onMainWindowCreated,
                     Qt::DirectConnection);

    createMainWindow();

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
    qmlRegisterType<MainWindow>("Crystal", 1, 0, "MainWindow");
}

void Application::initializeQmlEngine()
{
    mpQmlEngine = new QQmlApplicationEngine();
}


void Application::createMainWindow()
{
    QObject::connect(mpQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);
    mpQmlEngine->load(mmainUrl);
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if (!obj && mmainUrl == objUrl)
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
                     this, &Application::initialize3D,
                     Qt::DirectConnection);

    QObject::connect(mpMainWindow, &MainWindow::fileItemCreated,
                     mpPluginManager, &PluginManager::onFileItemCreated,
                     Qt::DirectConnection);
    QObject::connect(mpMainWindow, &MainWindow::fileItemClicked,
                     mpPluginManager, &PluginManager::onFileItemClicked,
                     Qt::DirectConnection);

    mpPluginManager->performPluginsInitQMLDesc(mpQmlEngine);

    mpMainWindow->initializePluginsUI(mpPluginManager->pluginsInfoList());
}

void Application::initialize3D()
{
    mpPluginManager->performPluginsInit3D(mpMainWindow->mapController());
}

