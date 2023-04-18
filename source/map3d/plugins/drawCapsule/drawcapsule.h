#ifndef DRAWCapsule_H
#define DRAWCapsule_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "mapcontroller.h"
#include "plugininterface.h"
#include "capsule.h"
#include "capsuleProperties.h"

#define DRAW_LAYER_NAME "Capsule"

class DrawCapsule : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCapsule.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCapsule(QObject *parent = nullptr);
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup(MapController *mapController, UIHandle *uiHandle) override;

    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void startDraw(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event);
    void cancelDrawing(QMouseEvent* event);

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);
    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    QQmlEngine *mQmlEngine;
    UIHandle *mUiHandle;
    MapController *mMapcontroller;
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Capsule* mCapsule{nullptr};
    bool mEnterCapsuleZone{false};

    CapsuleProperties *mCapsuleProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCapsule_H
