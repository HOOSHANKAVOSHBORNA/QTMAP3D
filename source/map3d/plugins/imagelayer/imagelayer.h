#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include "plugininterface.h"

class ImageLayer : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "imagelayer.json")
    Q_INTERFACES(PluginInterface)
public:
    ImageLayer(QWidget *parent = nullptr);
    ~ImageLayer()override{}
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    void onToolboxItemClicked(const QString &name, const QString &category) override;
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;
private:
    void addXYZ();
    void addArcGIS();
    void addGDAL();
    void addTMS();
    void addWMS();
private:
    MapItem* mMapItem{nullptr};
};
#endif // IMAGELAYER_H
