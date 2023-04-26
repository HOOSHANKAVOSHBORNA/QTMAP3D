#ifndef DRAWCylinder_H
#define DRAWCylinder_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "mapcontroller.h"
#include "plugininterface.h"
#include "cylinder.h"
#include "cylinderProperties.h"

#define DRAW_LAYER_NAME "Cylinder"

class DrawCylinder : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCylinder.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCylinder(QObject *parent = nullptr);
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup(MapController *mapController, UIHandle *uiHandle) override;

    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void startDraw(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event);
    void cancelDrawing(QMouseEvent* event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    QQmlEngine *mQmlEngine;
    UIHandle *mUiHandle;
    MapController *mMapcontroller;
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Cylinder* mCylinder{nullptr};
    bool mEnterCylinderZone{false};

    CylinderProperties *mCylinderProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCylinder_H
