#ifndef DRAWRECT_H
#define DRAWRECT_H
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "rect.h"
#include "rectproperties.h"

#define RECT "Rect"

class DrawRect : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawRect.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawRect(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onRectItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Rect> mRect{nullptr};
    RectProperties *mRectProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mRectLayer;
    static int mCount;
};
#endif // DRAWRECT_H
