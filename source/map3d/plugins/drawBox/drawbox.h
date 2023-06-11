#ifndef DRAWBOX_H
#define DRAWBOX_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

#include "plugininterface.h"
#include "box.h"
#include "boxProperties.h"

#define DRAW_LAYER_NAME "box"

class DrawBox : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawBox.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawBox(QObject *parent = nullptr);
    //bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup() override;

//    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onBoxItemCheck (bool check);
private:
    bool startDraw(const osgGA::GUIEventAdapter &ea);
    bool finishDrawing(const osgGA::GUIEventAdapter &ea);
    bool cancelDrawing(const osgGA::GUIEventAdapter &ea);
    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Box* mBox{nullptr};
    bool mEnterBoxZone{false};

    BoxProperties *mBoxProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWBOX_H
