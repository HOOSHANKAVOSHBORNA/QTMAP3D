#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include "plugininterface.h"
#include "circle.h"
#include "circleproperties.h"

#define DRAW_LAYER_NAME "Circle"

class DrawCircle : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCircle.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawCircle(QObject *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup() override;

    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;


private:
    bool startDraw(const osgGA::GUIEventAdapter& event);
    bool finishDrawing(const osgGA::GUIEventAdapter& event);
    bool cancelDrawing(const osgGA::GUIEventAdapter& event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Circle* mCircle{nullptr};
    CircleProperties* mCircleProperties{nullptr};


    osgEarth::Annotation::SphereDragger* mCircleHdragger;

    bool mEnterCircleZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCIRCLE_H
