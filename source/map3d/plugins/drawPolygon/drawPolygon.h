#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "polygonproperties.h"
#include "polygon.h"

#define POLYGON "Polygon"
#define MEASUREAREA "Area"

class DrawPolygon : public DrawShape
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawPolygon.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawPolygon(QObject *parent = nullptr);
//    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
//    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup() override;

//    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onPolygonItemCheck(bool check);
    void onMeasureAreaItemCheck(bool check);
private:
//    void startDraw(const osgGA::GUIEventAdapter& event);
//    bool drawing(const osgGA::GUIEventAdapter& event);
//    void cancelDraw();
//    void finishDraw();
//    bool mouseMoveDrawing(const osgGA::GUIEventAdapter& event);
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void tempDrawing(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

    //osgEarth::Annotation::PlaceNode* makeIconNode();

private:
//    enum class DrawingState{START, DRAWING, FINISH};
//    DrawingState mDrawingState;
    enum class Type{NONE, POLYGONN, AREA};
    Type mType;
    Polygon* mPolygon{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mPolygonLayer;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mAreaLayer;
    //bool mAreaMode{false};
    static int mCount;

    PolygonProperties* mPolygonProperties{nullptr};

//    bool mEnterPolygonZone{false};
    //bool mShowArea{false};
//    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};

};

#endif // DRAWPOLYGON_H
