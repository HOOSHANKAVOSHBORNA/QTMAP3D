
#ifndef CPTESTPLUGIN_H
#define CPTESTPLUGIN_H

#include "plugininterface.h"

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osg/Shader>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>

class Test1Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "test1plugin.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Test1Plugin();

public:
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc) override;
    virtual void onSideItemCreated(int index, QObject *pSideItem) override;
    virtual void onToolboxItemClicked(const QString& name, const QString& category) override;

    virtual bool initialize3D(MapController *pMapController) override;
private:
    osgEarth::Annotation::ModelNode* makeBackground(MapController *pMapController, float radius);
private:
    MapController *mMapController;
};

#endif // CPTESTPLUGIN_H
