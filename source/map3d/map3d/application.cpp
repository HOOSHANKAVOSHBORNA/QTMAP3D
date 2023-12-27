
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include <QDir>
#include <QQuickItem>
#include <QQmlComponent>

#include "application.h"
#include "mainwindow.h"
#include "listWindow.h"
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
    qmlRegisterType<MainWindow>("Crystal", 1, 0, "CMainWindow");
    qmlRegisterType<ListWindow>("Crystal", 1, 0, "CListWindow");
    qmlRegisterType<Authenticator>("Crystal", 1, 0, "Authenticator");

    initializeQmlEngine();
    initializeDefenseDataManager();

    mQmlEngine->load(QStringLiteral("qrc:///MainWindow.qml"));
    mQmlEngine->load(QStringLiteral("qrc:///ListWindow.qml"));
    // mQmlEngine->load(QStringLiteral("qrc:///LoginPage.qml"));
}

void Application::show()
{
    if (mIsReady) {
        // mMainWindow->show();
    } else {
        QObject::connect(this, &Application::ready, [this]() {
            // mMainWindow->show();
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

void Application::initializeQmlEngine()
{
    mQmlEngine = new QQmlApplicationEngine();
    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &Application::onQmlObjectCreated,
                     Qt::DirectConnection);

    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreationFailed,[]( const QUrl &url){
        qDebug()<<"Can not create: "<< url.toString();
    });
}

void Application::initializeDefenseDataManager()
{
    //TODO set parent
    mDefenseDataManager = new DefenseDataManager;
}

void Application::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if(!obj){
        qDebug()<<"Can not create: "<< objUrl.toString();
        QCoreApplication::exit(-1);
        return;
    }
    qDebug()<<"Load: "<< objUrl.toString();

    Authenticator *authenticator = qobject_cast<Authenticator*>(obj);
    MainWindow *mainWnd = qobject_cast<MainWindow*>(obj);
    ListWindow *listWnd = qobject_cast<ListWindow*>(obj);

    // if (authenticator){
        // mAuthenticator = authenticator;
        mServiceManager = new ServiceManager();
        mNetworkManager = new NetworkManager(mServiceManager);
        // mAuthenticator->setServiceManager(mServiceManager);
        mNetworkManager->start();
        // connect(mServiceManager, &ServiceManager::signInResponseReceived, [&](bool status, int role){
        //     if (status && !mMainWindow) {
                // mQmlEngine->load(QStringLiteral("qrc:///MainWindow.qml"));
                // mQmlEngine->load(QStringLiteral("qrc:///ListWindow.qml"));
                // mAuthenticator->hide();
        //         mRole = static_cast<UserRoles>(role);
        //     }
        // });
    // }
    if (mainWnd) {
        mMainWindow = mainWnd;
        mMainWindow->initComponent();
    }

    if (listWnd) {
        mListWindow = listWnd;

        mMainWindow->setListWindow(mListWindow);
        onUICreated();
    }
}

void Application::onUICreated()
{
    connect(mServiceManager, &ServiceManager::layerDataReceived, [&](CompositeAnnotationLayer *layer){
            mMainWindow->getMapItem()->getMapObject()->addLayer(layer);
        });
    connect(mServiceManager, &ServiceManager::clearMap, mMainWindow->getMapItem()->getMapObject(), &MapObject::clearParenticLayers);
    mPluginManager->loadPlugins();
    mPluginManager->setup();
    emit defenseDataManagerInitialized(mDefenseDataManager);

    mIsReady = true;
    emit ready();
}

ServiceManager *Application::serviceManager() const
{
    return mServiceManager;
}


