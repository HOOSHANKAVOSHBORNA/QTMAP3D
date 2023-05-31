#ifndef DRAWRECT_H
#define DRAWRECT_H
#include <osgEarthAnnotation/ModelNode>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/PlaceNode>
#include "mapItem.h"
#include "plugininterface.h"
#include "rect.h"
#include "rectproperties.h"

#define DRAW_LAYER_NAME "Rect"

class DrawRect : public PluginInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawRect.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit DrawRect(QObject *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;

//    virtual void mousePressEvent(QMouseEvent* event) override;
//    virtual void mouseMoveEvent(QMouseEvent* event) override;
private:
    void startDraw(QMouseEvent* event);
    void cancelDraw();
    void finishDraw();

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    MapItem* mMapItem {nullptr};
    QQmlEngine* mQmlEngine {nullptr};
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Rect* mRect{nullptr};

    RectProperties* mRectProperties{nullptr};

    UIHandle* mUiHandle{nullptr};

    bool mEnterRectZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWRECT_H
