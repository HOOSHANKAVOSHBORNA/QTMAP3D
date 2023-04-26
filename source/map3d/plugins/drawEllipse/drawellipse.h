#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H

#include <osgEarthAnnotation/ModelNode>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/PlaceNode>
#include "mapcontroller.h"
#include "plugininterface.h"
#include "ellipse.h"

#define DRAW_LAYER_NAME "Ellipse"

class DrawEllipse : public PluginInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "ellipse.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit DrawEllipse(QObject *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
private:
    void startDraw(QMouseEvent* event);
    void cancelDraw();
    void finishDraw();

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapController* mMapController {nullptr};
    QQmlEngine* mQmlEngine {nullptr};
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Ellipse* mEllipse{nullptr};

    UIHandle* mUiHandle{nullptr};

    bool mEnterEllipseZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};

};

#endif // DRAWELLIPSE_H
