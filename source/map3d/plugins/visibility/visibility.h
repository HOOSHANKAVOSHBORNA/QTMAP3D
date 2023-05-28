
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
#include <osgEarthAnnotation/AnnotationUtils>

#include <QMouseEvent>

class Visibility : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "visibility.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Visibility(QObject *parent = nullptr);

public:
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;
private slots:
    void onMouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos);
private:
    osgEarth::Annotation::FeatureNode* makepolygan(QVector<osg::Vec3d> vertices);
    osgEarth::Annotation::ModelNode* makeBackground(float radius);
    osgEarth::Annotation::PlaceNode *makeIconNode();
private:
    osg::ref_ptr<osgEarth::Annotation::ModelNode>  mBackVisibilityNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::FeatureNode> mVisibilityNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    MapItem* mMapItem{nullptr};
};

#endif // CPTESTPLUGIN_H
