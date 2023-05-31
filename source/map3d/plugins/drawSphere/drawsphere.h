#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/PlaceNode>
#include "osgEarth/ModelLayer"
#include <osgEarth/GLUtils>

#include "plugininterface.h"
#include "sphereproperties.h"
#include "mapItem.h"
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
    virtual bool setup(MapItem *mapController, UIHandle *uiHandle) override;

    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

private:
    void startDraw(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event);
    void cancelDrawing(QMouseEvent* event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapItem* mMapcontroller {nullptr};
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
