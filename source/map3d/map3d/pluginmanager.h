
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
    ~PluginManager();

public:
    void loadPlugins();

    QMap<QString, PluginInterface *> pluginsMap() const;
signals:
    void plugunCount(int count);
    void pluginMessage(QString message, bool isError);
    void setupFinished();

private:
    void setup();
    void parsePlugin(const QString &pluginFileName, const QDir &pluginsDir);
    void loadPlugin(const QString &pluginFileName, const QDir &pluginsDir);
    void unLoadPlugins();

private:
    friend EventHandler;

    QMap<QString, PluginInterface*> mPluginsMap;
    QVector<QString> mLoadedPlugins;
    QStringList mPluginFileNameList;
    QStringList mLoadedPluginList;
    QPluginLoader *mPluginLoader;
    int mIndex{0};
    QDir mPluginsDir;
    QTimer *mPluginTimer;
};

#endif // PluginManager_H
