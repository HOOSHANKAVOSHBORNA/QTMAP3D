#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>
#include <list>

#include "userManager.h"
#include "pluginmanager.h"
#include "networkManager.h"
#include "splash.h"


class MainWindow;
class ListWindow;
class QQmlApplicationEngine;

class Application : public QObject
{
    Q_OBJECT

private:
    explicit Application();

public:
    static Application *instance();
    static void performStartupConfiguration();
    void initialize();
    void show();
    inline MainWindow *mainWindow() const { return mMainWindow; }
    inline QQmlApplicationEngine *qmlEngine() const { return mQmlEngine; }
    inline PluginManager *pluginManager() const { return mPluginManager; }

    ServiceManager *serviceManager() const;
    void showSplash();

signals:
    void ready();

private:
    static void initializeSurfaceFormat();
    void initializeQmlEngine();

private slots:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);
    void onUICreated();

private:
    QQmlApplicationEngine *mQmlEngine = nullptr;
    MainWindow *mMainWindow = nullptr;
    ListWindow *mListWindow = nullptr;

    PluginManager *mPluginManager = nullptr;
    ServiceManager *mServiceManager{nullptr};

    UserManager *mUserManager{nullptr};
    Splash *mSplash{nullptr};
    NetworkManager *mNetworkManager{nullptr};

    bool mIsReady{false};
};

#endif // Application_H
