
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "rect.h"
#include "rectproperties.h"
#include "compositeAnnotationLayer.h"



#define RECT "Rect"
class ParenticAnnotationLayer;
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
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
    void createProperty();

private:
    osg::ref_ptr<Rect> mRect{nullptr};
    RectProperties *mRectProperties{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mCompositeRectLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mRectLayer;
    static int mCount;




};
