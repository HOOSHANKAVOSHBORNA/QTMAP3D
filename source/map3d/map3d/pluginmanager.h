
#ifndef PluginManager_H
#define PluginManager_H

#include <QObject>
#include <QQmlEngine>
#include <list>
#include <QMap>
#include <utility>
#include <tuple>

#include "plugininterface.h"

class PluginManager;
class QDir;
class EventHandler: public osgGA::GUIEventHandler{
public:
    EventHandler(PluginManager *pluginManager);
    virtual bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    PluginManager *mPluginManager;
};

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);

public:
    void extracted(QDir &pluginsDir);
    void loadPlugins();
    void setup();

private:
    void parsePlugin(const QString &pluginFileName, const QDir &pluginsDir);
    void loadPlugin(const QString &pluginFileName, const QDir &pluginsDir);

private:
    friend EventHandler;
    std::list<PluginInterface*> mPluginsInfoList;
    QStringList mPluginFileNameList;
    QStringList mLoadedPluginList;
};

#endif // PluginManager_H
