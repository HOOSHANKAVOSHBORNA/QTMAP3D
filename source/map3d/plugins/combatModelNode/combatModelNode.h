#ifndef COMBATMODELNODE_H
#define COMBATMODELNODE_H

#include <QObject>
#include "combatManager.h"
#include "plugininterface.h"
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include "simpleModelNode.h"
#include "dataManager.h"
#include "combatListModel.h"

#define COMBATMODELNODE "Combat"
#define TANK "Tank"


class CombatModelNode : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "combatModelNode.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        NONE,
        READY,
        MOVING,
        CANCEL,
        CONFIRM,
        MANUALADD,
        MANUALREMOVE
    };

public:
    explicit CombatModelNode(QObject *parent = nullptr);
    ~CombatModelNode();
    bool setup();
    void makeIconNode(const QString &fileName);
    osgEarth::Annotation::PlaceNode *iconNode() const;
    CombatModelNode::State state() const;
    void setState(CombatModelNode::State newState);

    bool mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseReleaseEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseDragEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

    static osgEarth::Symbology::Style &getDefaultStyle();

public slots:
    void onTankItemCheck (bool check);
    void onModeChanged(bool is3DView);
    void onTargetMenuChecked();
    void onAttackMenuChecked();
    void onAddManuallyChecked();
    void onCombatListClosed();
    void onRemoveManuallyChecked();

protected:
    void initModel(osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancel();

private:
    SimpleModelNode* pick(float x, float y);
    osgEarth::Annotation::ModelNode *getDragModel();
    NodeData* sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc, std::string iconSrc,osgEarth::GeoPoint geoPos);

private:
    CombatManager *mCombatManager;
    CombatList *mCombatList;
    QList<int> mBulletID;
    static int mCount;
    bool mIs3D;
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::ModelNode> mDragModelNode;
    osg::ref_ptr<SimpleModelNode> mAttackerNode;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mCombatModelNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mAttackNodeLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    NodeData* mNodeData{nullptr};
    DataManager *mDataManager{nullptr};
    QQmlEngine *mEngine{nullptr};
};

#endif // COMBATMODELNODE_H
