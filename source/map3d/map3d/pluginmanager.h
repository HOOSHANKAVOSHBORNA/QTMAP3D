
#ifndef PluginManager_H
#define PluginManager_H

#include <QObject>
#include "plugininterface.h"

class PluginManager;
class QDir;
class EventHandler: public osgGA::GUIEventHandler{
public:
    EventHandler(PluginManager *pluginManager);
    virtual bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    PluginManager *mPluginManager;
    QPointF mCurrentPressPoint;
};

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);

public:
    void extracted(QDir &pluginsDir);
    void setQmlEngine(QQmlEngine *engine);
    void loadPlugins();
    void setup();

    QMap<QString, PluginInterface *> pluginsMap() const;
signals:
    void pluginLoading(QString pluginName);
    void pluginLoadError(QString errorStr);
//    void pluginsLoaded();
private:
    void parsePlugin(const QString &pluginFileName, const QDir &pluginsDir);
    void loadPlugin(const QString &pluginFileName, const QDir &pluginsDir);

private:
    friend EventHandler;

    QMap<QString, PluginInterface*> mPluginsMap;
    QStringList mPluginFileNameList;
    QStringList mLoadedPluginList;
};

#endif // PluginManager_H
