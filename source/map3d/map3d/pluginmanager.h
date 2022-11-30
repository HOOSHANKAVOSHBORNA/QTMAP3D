
#ifndef PluginManager_H
#define PluginManager_H

#include <QObject>
#include <QQmlEngine>
#include <list>
#include <QMap>
#include <utility>
#include <tuple>

#include "plugininterface.h"

class MapController;
class PluginInterface;

struct CrystalPluginInfo
{
    PluginInterface *interface = nullptr;
    PluginQMLDesc   *qmlDesc    = nullptr;
    int              sideItemIndex      = -1;
};

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);

signals:

public:
    void loadPlugins();
    void performPluginsInitQMLDesc(QQmlEngine *qmlEngine);
    void performPluginsInit3D(MapController *mapController);

    std::list<CrystalPluginInfo>& pluginsInfoList();

public slots:
    void onSideItemCreated(int index, QObject *sideItem);
    void onToolboxItemCreated(const QString& name,
                              const QString& category,
                              QObject *menuItem,
                              PluginInterface *interface);
    void onToolboxItemClicked(const QString& name,
                              const QString& category);

private:
    std::list<CrystalPluginInfo> mPluginsInfoList;
    QMap<QString, QMap<QString, PluginInterface*>> mToolboxItemsMap;
};

#endif // PluginManager_H
