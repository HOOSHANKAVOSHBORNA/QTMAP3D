#ifndef DRAWCylinder_H
#define DRAWCylinder_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "mapItem.h"
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
    bool setup(MapItem *mapController, UIHandle *uiHandle) override;

    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

private:
    bool startDraw(const osgGA::GUIEventAdapter& event);
    bool finishDrawing(const osgGA::GUIEventAdapter& event);
    bool cancelDrawing(const osgGA::GUIEventAdapter& event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    QQmlEngine *mQmlEngine;
    UIHandle *mUiHandle;
    MapItem *mMapcontroller;
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Cylinder* mCylinder{nullptr};
    bool mEnterCylinderZone{false};

    CylinderProperties *mCylinderProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCylinder_H
