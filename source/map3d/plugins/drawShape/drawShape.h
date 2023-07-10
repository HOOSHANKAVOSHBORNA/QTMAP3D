#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

#include <QMouseEvent>
//#include <osgEarthAnnotation/FeatureNode>
//#include <osgEarthAnnotation/ModelNode>
//#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"

//#include <osgEarthAnnotation/AnnotationLayer>
//#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#include "compositeLayer.h"

#define CATEGORY "Draw"
#define M_CATEGORY "Measurement"


class DrawShape : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawShape.json")
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
public:
    explicit DrawShape(QObject *parent = nullptr);
    ~DrawShape()override;
    virtual bool setup() override;
    void makeIconNode(const QString &fileName);
    osgEarth::Annotation::PlaceNode *iconNode() const;
    DrawShape::State state() const;
    void setState(DrawShape::State newState);
    CompositeAnnotationLayer *shapeLayer();
    CompositeAnnotationLayer *measureLayer();

    virtual bool frameEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
    virtual bool keyPressEvent        (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
    virtual bool keyReleaseEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
    virtual bool mousePressEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    virtual bool mouseReleaseEvent    (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
    virtual bool mouseMoveEvent       (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    virtual bool wheelEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override {return false;}
protected:
    virtual void initDraw(const osgEarth::GeoPoint &geoPos){};
    virtual void drawing(const osgEarth::GeoPoint &geoPos){};
    virtual void tempDrawing(const osgEarth::GeoPoint &geoPos){};
    virtual void confirmDraw();
    virtual void cancelDraw(){};
private:
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    CompositeAnnotationLayer* mShapeLayer{nullptr};
    CompositeAnnotationLayer* mMeasureLayer{nullptr};

};

#endif // DRAWSHAPE_H
