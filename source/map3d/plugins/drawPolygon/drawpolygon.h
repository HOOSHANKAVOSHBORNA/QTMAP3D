#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/PlaceNode>
#include "mapcontroller.h"
#include "plugininterface.h"
#include "polygonproperties.h"
#include "polygon.h"

#define DRAW_LAYER_NAME "Polygon"

class DrawPolygon : public PluginInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawPolygon.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawPolygon(QObject *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    void startDraw(QMouseEvent* event);
    void drawing(QMouseEvent* event);
    void cancelDraw();
    void finishDraw(QMouseEvent* event);
    void mouseMoveDrawing(QMouseEvent *event);

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapController* mMapController {nullptr};
    QQmlEngine* mQmlEngine {nullptr};
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Polygon* mPolygon{nullptr};

    UIHandle* mUiHandle{nullptr};
    PolygonProperties* mPolygonProperties{nullptr};

    bool mEnterPolygonZone{false};    
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};

};

#endif // DRAWPOLYGON_H
