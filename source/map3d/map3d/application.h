#ifndef Application_H
#define Application_H

#include <QObject>
#include <QUrl>
#include <list>

#include "plugininterface.h"
#include "pluginmanager.h"


class MainWindow;
class CRYSTAL_PLUGIN_QML_INPUT_DESC;
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

private slots:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);
    void onMainWindowCreated();
    void initialize3D();

signals:

private:
    QQmlApplicationEngine *mpQmlEngine = nullptr;
    const QUrl mmainUrl{QStringLiteral("qrc:///Main.qml")};
    MainWindow *mpMainWindow = nullptr;

    PluginManager *mpPluginManager = nullptr;
};

#endif // Application_H
