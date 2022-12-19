#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>
#include <list>

#include "plugininterface.h"
#include "pluginmanager.h"
#include "networkmanager.h"


class MainWindow;
class QQmlApplicationEngine;

class Application : public QObject
{
    Q_OBJECT

private:
    explicit Application();

public:
    static Application *instance();
    int main(int argc, char **argv);

signals:
    void  mainWindowCreated();


private:
    void initializeSurfaceFormat();
    void registerTypes();
    void initializeQmlEngine();
    void createMainWindow();
    void createListWindow();
    void initializeNetworkManager();

private slots:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);
    void onMainWindowCreated();
    void setup();

public:
    inline NetworkManager * networkManager() const {return mNetworkManager;}
    inline MainWindow *mainWindow() const { return mpMainWindow; }

private:
    QQmlApplicationEngine *mpQmlEngine = nullptr;
    const QUrl mMainWindowUrl{QStringLiteral("qrc:///MainWindow.qml")};
    const QUrl mListWindowUrl{QStringLiteral("qrc:///ListWindow.qml")};
    MainWindow *mpMainWindow = nullptr;

    PluginManager *mpPluginManager = nullptr;
    NetworkManager *mNetworkManager = nullptr;
};

#endif // Application_H
