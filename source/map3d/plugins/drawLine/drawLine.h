#ifndef LINE_H
#define LINE_H


#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QMouseEvent>
#include <QPainter>

#include "drawShape.h"
#include "lineNode.h"
#include "lineProperty.h"
#include "measureHeight.h"

#define POLYLINE "Polyline"
#define RULER "Ruler"
#define MEASUREHEIGHT "Height"
#define SLOPE "Slope"

class DrawLine: public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawLine.json")
    Q_INTERFACES(PluginInterface)
public:
    enum class Type{NONE, LINE, RULERR, HEIGHT, SLOPEE};
public:
    explicit DrawLine(QWidget *parent = nullptr);
    bool setup() override;

private slots:
    void onLineItemCheck(bool check);
    void onRulerItemCheck(bool check);
    void onHeightItemCheck(bool check);
    void onSlopeItemCheck(bool check);
    void addUpdateLineNode(PolyLineData *lineNodeData);
private:
    void onItemChecked(Type type, bool check);
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void tempDrawing(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
private:
    Type mType;
    osg::ref_ptr<LineNode> mLine{nullptr};
    MeasureHeight *mMeasureHeight{nullptr};
    LineProperty *mLineProperty = nullptr;
    osg::ref_ptr<ParenticAnnotationLayer> mLineLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mRulerLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mHeightLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mSlopeLayer;
    QMap<int, osg::ref_ptr<LineNode>> mLineNodeMap;
    static int mCount;
};

#endif // LINE_H
