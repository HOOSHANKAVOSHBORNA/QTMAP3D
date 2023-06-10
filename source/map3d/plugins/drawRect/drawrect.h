#ifndef DRAWRECT_H
#define DRAWRECT_H
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
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
    bool setup() override;

    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    void startDraw(const osgGA::GUIEventAdapter& event);
    bool cancelDraw(const osgGA::GUIEventAdapter& event);
    bool finishDraw(const osgGA::GUIEventAdapter& event);

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Rect* mRect{nullptr};

    RectProperties* mRectProperties{nullptr};

    bool mEnterRectZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWRECT_H
