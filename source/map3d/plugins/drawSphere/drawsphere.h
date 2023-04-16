#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/PlaceNode>
#include "osgEarth/ModelLayer"
#include <osgEarth/GLUtils>

#include "plugininterface.h"
#include "sphereproperties.h"
#include "mapcontroller.h"
#include "sphereNode.h"

#define DRAW_LAYER_NAME "Sphere"
class drawSphere : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawSphere.json")
    Q_INTERFACES(PluginInterface)
public:
    drawSphere(QObject *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    virtual bool setup(MapController *mapController, UIHandle *uiHandle) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

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
    SphereNode* mSphere{nullptr};
    UIHandle* mUiHandle{nullptr};
    SphereProperties* mSphereProperties{nullptr};


    bool mEnterSphereZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCIRCLE_H
