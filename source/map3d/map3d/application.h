#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>
#include <list>

#include "pluginmanager.h"
#include "defenseDataManager.h"


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

    inline DefenseDataManager *defenseDataManager() const{ return mDefenseDataManager; }

    ServiceManager *serviceManager() const;

signals:
    void ready();
    void defenseDataManagerInitialized(DefenseDataManager *defenseDataManager);

private:
    static void initializeSurfaceFormat();
    void initializeQmlEngine();
    void initializeDefenseDataManager();

private slots:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);
    void onUICreated();

private:
    QQmlApplicationEngine *mQmlEngine = nullptr;
    MainWindow *mMainWindow = nullptr;
    ListWindow *mListWindow = nullptr;

    PluginManager *mPluginManager = nullptr;
    DefenseDataManager *mDefenseDataManager{nullptr};
    ServiceManager *mServiceManager{nullptr};

    bool mIsReady{false};
};

#endif // Application_H
