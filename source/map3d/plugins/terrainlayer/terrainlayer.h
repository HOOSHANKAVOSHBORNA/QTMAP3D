#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include "plugininterface.h"

class TerrainLayer: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "terrainlayer.json")
    Q_INTERFACES(PluginInterface)
public:
    TerrainLayer(QWidget *parent = nullptr);
    ~TerrainLayer()override{}
    void setUpUI() override;
private:
    void addGDAL();
    void addArcGIS();
    void addWCS();
    void addTMS();
};

#endif // TERRAINLAYER_H
