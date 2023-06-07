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

signals:
    void uiCreated();
//    void  mainWindowCreated();
//    void  listWindowCreated();
    void defenseDataManagerInitialized(DefenseDataManager *defenseDataManager);


private:
    static void initializeSurfaceFormat();
    void registerTypes();
    void initializeQmlEngine();
    void createMainWindow();
    void createListWindow();
    //void initializeNetworkManager();
    void initializeDefenseDataManager();

private slots:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);
//    void onMainWindowCreated();
//    void onListWindowCreated();
//    void onAllWindowsCreated();
    void setup();
    void onUICreated();

public:
//    inline NetworkManager * networkManager() const {return mNetworkManager;}
    inline MainWindow *mainWindow() const { return mMainWindow; }
    inline QQmlApplicationEngine *qmlEngine() const { return mQmlEngine; }
    inline PluginManager *pluginManager() const { return mPluginManager; }

//    inline bool isMainWindowReady() const { return mMainWindowIsReady; }
//    inline bool isListWindowReady() const { return mListWindowIsReady; }

    inline DefenseDataManager *defenseDataManager() const{ return mDefenseDataManager; }

private:
    QQmlApplicationEngine *mQmlEngine = nullptr;
    const QUrl mMainWindowUrl{QStringLiteral("qrc:///MainWindow.qml")};
    const QUrl mListWindowUrl{QStringLiteral("qrc:///ListWindow.qml")};
    MainWindow *mMainWindow = nullptr;
    ListWindow *mListWindow = nullptr;

    PluginManager *mPluginManager = nullptr;
//    NetworkManager *mNetworkManager = nullptr;
    DefenseDataManager *mDefenseDataManager{nullptr};


//    bool mMainWindowIsReady = false;
//    bool mListWindowIsReady = false;
    bool mUIIsReady{false};
};

#endif // Application_H
