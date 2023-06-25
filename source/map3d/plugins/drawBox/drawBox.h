#ifndef DRAWBOX_H
#define DRAWBOX_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

#include "drawShape.h"
#include "box.h"
#include "boxProperties.h"

#define BOX "Box"

class DrawBox : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawBox.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawBox(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onBoxItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
private:
    osg::ref_ptr<Box> mBox{nullptr};
    BoxProperties *mBoxProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mBoxLayer;
    static int mCount;
};

#endif // DRAWBOX_H
