#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>
#include <list>

#include "connectionConfiguration.h"
#include "loadingPage.h"
#include "userManager.h"
#include "pluginmanager.h"
#include "networkManager.h"


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
    inline MainWindow *mainWindow() const { return mMainWindow; }
    inline QQmlApplicationEngine *qmlEngine() const { return mQmlEngine; }
    inline PluginManager *pluginManager() const { return mPluginManager; }

    ServiceManager *serviceManager() const;

    void initializeQmlEngine();
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);

    void setPageIndex(int index);

private:
    static void initializeSurfaceFormat();

private slots:
    void clearMainWindow();

private:
    QQmlApplicationEngine *mQmlEngine = nullptr;
    MainWindow *mMainWindow = nullptr;

    PluginManager *mPluginManager = nullptr;
    ServiceManager *mServiceManager{nullptr};

    UserManager *mUserManager{nullptr};
    ConnectionConfiguration *mConnectionConfig{nullptr};
    LoadingPage *mLoadingPage{nullptr};
    NetworkManager *mNetworkManager{nullptr};

    QQuickWindow *mApplicationWindow = nullptr;
};

#endif // Application_H
