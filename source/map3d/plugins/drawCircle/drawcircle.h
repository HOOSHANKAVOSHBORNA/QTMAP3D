#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/ModelNode>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include "mapcontroller.h"
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
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;


private:
    void startDraw(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event);
    void cancelDrawing(QMouseEvent* event);

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);
    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapController* mMapcontroller {nullptr};
    QQmlEngine* mQmlEngine {nullptr};
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Circle* mCircle{nullptr};
    UIHandle* mUiHandle{nullptr};
    CircleProperties* mCircleProperties{nullptr};


    osgEarth::Annotation::SphereDragger* mCircleHdragger;

    bool mEnterCircleZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCIRCLE_H
