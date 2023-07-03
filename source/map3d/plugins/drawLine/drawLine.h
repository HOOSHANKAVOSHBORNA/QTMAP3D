#ifndef LINE_H
#define LINE_H

#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include "drawShape.h"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <QQmlEngine>
#include <QQmlComponent>
#include <lineNode.h>
#include <QPainter>
#include <lineProperties.h>
#include <measureHeight.h>

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
    explicit DrawLine(QWidget *parent = nullptr);
    bool setup() override;

protected:
//    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onLineItemCheck(bool check);
    void onRulerItemCheck(bool check);
    void onHeightItemCheck(bool check);
    void onSlopeItemCheck(bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void tempDrawing(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

//    void startDrawLine();
//    void startDrawMeasureHeight();
//    bool drawingMeasureHeight(const osgGA::GUIEventAdapter& event);
//    bool mouseMoveMeasureHeightDrawing(const osgGA::GUIEventAdapter& event);
//    bool drawingLine(const osgGA::GUIEventAdapter& event);
//    bool cancelDrawingLine(const osgGA::GUIEventAdapter& event);
//    bool mouseMoveDrawing(const osgGA::GUIEventAdapter& event);
//    bool finishDrawing(const osgGA::GUIEventAdapter& event, osg::Node *nodeEditor = nullptr);
//    osgEarth::Annotation::PlaceNode *makeIconNode();


private:
//    enum class DrawingState{START, DRAWING, FINISH};
//    DrawingState mDrawingState;


    enum class Type{NONE, LINE, RULERR, HEIGHT, SLOPEE};
    Type mType;
    osg::ref_ptr<LineNode> mLine{nullptr};
    MeasureHeight *mMeasureHeight{nullptr};
    LineProperties *mLineProperties = nullptr;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mLineLayer;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mRulerLayer;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mHeightLayer;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mSlopeLayer;
    static int mCount;
//    bool mEnterLineZone{false};
//    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
//    osg::ref_ptr<osg::Image> mIcon;
};

#endif // LINE_H
