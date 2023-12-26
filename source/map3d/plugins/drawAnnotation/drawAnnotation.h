#ifndef DRAWANNOTATION_H
#define DRAWANNOTATION_H

#include <QMouseEvent>
//#include <osgEarthAnnotation/FeatureNode>
//#include <osgEarthAnnotation/ModelNode>
//#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"
#include "polygonProperty.h"
//#include <osgEarthAnnotation/AnnotationLayer>
//#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#include "compositeAnnotationLayer.h"
#include "mapItem.h"
#include "lineNode.h"
#include "lineProperty.h"
#include "measureHeight.h"
#include "polygon.h"
#define CATEGORY "Draw Annotation"

#define POLYLINE "Polyline"
#define POLYGON "Polygon"


class DrawAnnotation : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawAnnotation.json")
    Q_INTERFACES(PluginInterface)
public:
    enum class State{
        NONE,
        READY,
        DRAWING,
        EDIT,
        CANCEL,
        CONFIRM
    };

    enum class Type{NONE, LINE, RULERR, HEIGHT, SLOPEE, POLYGONN};
public:
    explicit DrawAnnotation(QObject *parent = nullptr);
    ~DrawAnnotation()override;
    void addUpdateLineNode(PolyLineData *lineNodeData);
    virtual bool setup() override;
    void makeIconNode(const QString &fileName);
    osgEarth::Annotation::PlaceNode *iconNode() const;
    DrawAnnotation::State state() const;
    void setState(DrawAnnotation::State newState);
    CompositeAnnotationLayer *shapeLayer();


    virtual bool mousePressEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    virtual bool mouseMoveEvent       (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;

private slots:
    /***********line*********/
    void onLineItemCheck(bool check);
    void onRulerItemCheck(bool check);
    void onHeightItemCheck(bool check);
    void onSlopeItemCheck(bool check);
    void LineNodeDataReceived(PolyLineData *lineNodeData);
    /*********polygon*********/
    void addUpdatePolygon(PolygonData *polygonData);
    void polygonDataReceived(PolygonData *polygonData);
    void onPolygonItemCheck (bool check);

protected:
    virtual void initDraw(const osgEarth::GeoPoint &geoPos);
    void onItemChecked(Type type, bool check);
    virtual void drawing(const osgEarth::GeoPoint &geoPos);
    virtual void tempDrawing(const osgEarth::GeoPoint &geoPos);
    virtual void confirmDraw();
    virtual void cancelDraw();

    void createProperty(QString name, QVariant property);
    void hideProperty();
private:
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    CompositeAnnotationLayer* mShapeLayer{nullptr};
    CompositeAnnotationLayer* mMeasureLayer{nullptr};
    QQuickItem *mItem{nullptr};
    Type mType;
    osg::ref_ptr<LineNode> mLine{nullptr};
    osg::ref_ptr<Polygon> mPolygon{nullptr};
    MeasureHeight *mMeasureHeight{nullptr};
    LineProperty *mLineProperty = nullptr;
    PolygonProperty *mPolygonProperty{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mLineLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mRulerLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mHeightLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mSlopeLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mPolygonLayer;
    QMap<int, osg::ref_ptr<LineNode>> mLineNodeMap;
    QMap<int, osg::ref_ptr<Polygon>> mPolygonMap;
    static int mCount;
};


#endif // DRAWANNOTATION_H
