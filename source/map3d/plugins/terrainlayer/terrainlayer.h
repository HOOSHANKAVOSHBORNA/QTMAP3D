#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include "plugininterface.h"
#include "mapItem.h"
class TerrainLayer: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "terrainlayer.json")
    Q_INTERFACES(PluginInterface)
public:
    TerrainLayer(QWidget *parent = nullptr);
    ~TerrainLayer()override{}
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    void onToolboxItemClicked(const QString &name, const QString &category) override;
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;
private:
    void addGDAL();
    void addArcGIS();
    void addWCS();
    void addTMS();
private:
    MapItem* mMapItem{nullptr};
};

#endif // TERRAINLAYER_H
