#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/PlaceNode>
#include "mapItem.h"
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
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;

//    virtual void mousePressEvent(QMouseEvent* event) override;
//    virtual void mouseMoveEvent(QMouseEvent* event) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    void startDraw(QMouseEvent* event);
    void drawing(QMouseEvent* event);
    void cancelDraw();
    void finishDraw();
    void mouseMoveDrawing(QMouseEvent *event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapItem* mMapItem {nullptr};
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
