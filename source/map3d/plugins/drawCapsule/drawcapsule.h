#ifndef DRAWCapsule_H
#define DRAWCapsule_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "mapItem.h"
#include "plugininterface.h"
#include "capsule.h"
#include "capsuleProperties.h"

#define DRAW_LAYER_NAME "Capsule"

class DrawCapsule : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCapsule.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCapsule(QObject *parent = nullptr);
//    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup() override;

//    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onCapsuleItemCheck (bool check);
private:
    bool startDraw(const osgGA::GUIEventAdapter& event);
    bool finishDrawing(const osgGA::GUIEventAdapter& event);
    bool cancelDrawing(const osgGA::GUIEventAdapter& event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Capsule* mCapsule{nullptr};
    bool mEnterCapsuleZone{false};

    CapsuleProperties *mCapsuleProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCapsule_H
