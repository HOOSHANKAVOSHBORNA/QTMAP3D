#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osg/ShapeDrawable>
#include <osgEarth/GLUtils>
#include "drawShape.h"
#include "ellipse.h"
#include "ellipseproperty.h"

#define ELLIPSE "Ellipse"

class DrawEllipse : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "ellipse.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawEllipse(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onEllipseItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;


private:
    osg::ref_ptr<Ellipse> mEllipse{nullptr};
    EllipseProperty *mEllipseProperty{nullptr};

    osg::ref_ptr<ParenticAnnotationLayer> mEllipseLayer;
    static int mCount;
    QQuickItem *mItem;


};
#endif // DRAWELLIPSE_H
