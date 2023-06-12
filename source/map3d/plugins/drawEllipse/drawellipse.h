#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "plugininterface.h"
#include "ellipse.h"
#include "ellipseproperties.h"

#define DRAW_LAYER_NAME "Ellipse"

class DrawEllipse : public PluginInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "ellipse.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit DrawEllipse(QObject *parent = nullptr);
//    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
//    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup() override;

    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onEllipseItemCheck(bool check);
private:
    void startDraw(const osgGA::GUIEventAdapter& event);
    void cancelDraw();
    void finishDraw();

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Ellipse* mEllipse{nullptr};

    EllipseProperties *mEllipseProperties{nullptr};

    bool mEnterEllipseZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};

};

#endif // DRAWELLIPSE_H
