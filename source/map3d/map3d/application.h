#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>

#include "connectionConfiguration.h"
#include "loadingPage.h"
#include "userManager.h"
#include "pluginmanager.h"
#include "networkManager.h"
#include "mainwindow.h"

// class MainWindow;
class ListWindow;
class QQmlApplicationEngine;




class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)

    Q_PROPERTY(UserManager* userManager READ userManager NOTIFY userManagerChanged)
    Q_PROPERTY(ConnectionConfiguration* connectionConfigCpp READ connectionConfigCpp NOTIFY connectionConfigCppChanged)
    Q_PROPERTY(LoadingPage* loadingPageCpp READ loadingPageCpp NOTIFY loadingPageCppChanged)
    Q_PROPERTY(MainWindow* mainPageCpp READ mainPageCpp NOTIFY mainPageCppChanged)
private:
    explicit Application();

public:
    static Application *instance();
    static void performStartupConfiguration();
    void initialize(QQmlApplicationEngine *newQmlEngine);

    inline MainWindow *mainWindow() const { return mMainWindow; }
    inline QQmlApplicationEngine *qmlEngine() const { return mQmlEngine; }
    inline PluginManager *pluginManager() const { return mPluginManager; }
    inline ServiceManager *serviceManager() const{return mServiceManager;}
    void setPageIndex(int index);
    inline int pageIndex() const{return mPageIndex;}

    Q_INVOKABLE UserManager* userManager();
    Q_INVOKABLE ConnectionConfiguration* connectionConfigCpp();
    Q_INVOKABLE LoadingPage* loadingPageCpp();
    Q_INVOKABLE MainWindow* mainPageCpp();

    Q_INVOKABLE void saveDataInFile();

signals:
    void pageIndexChanged();
    void userManagerChanged();
    void connectionConfigCppChanged();
    void loadingPageCppChanged();
    void mainPageCppChanged();

private:
    static void initializeSurfaceFormat();

private slots:
    void onLoadingPage();
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

//    QQuickWindow *mApplicationWindow = nullptr;
    int mPageIndex {0};
};

#endif // Application_H
