#ifndef COMBATMODELNODE_H
#define COMBATMODELNODE_H

#include <QObject>
#include "combatManager.h"
#include "plugininterface.h"
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include "simpleModelNode.h"
#include "combatMenu.h"

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
        ADD_ASSIGNMENT,
        REMOVE_ASSIGNMENT
    };

public:
    explicit CombatModelNode(QObject *parent = nullptr);
    ~CombatModelNode();
    bool setup();
    void makeIconNode(const QString &fileName);
    bool mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

public slots:
    void onModeChanged(bool is3DView);
    void onTargetMenuChecked();
    void onAttackMenuChecked();
    void onAddAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker);
    void onRemoveAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker);
    void onCloseMenuClicked();

private:
    SimpleModelNode* pick(float x, float y);

private:
    CombatManager *mCombatManager;
    CombatMenu *mCombatMenu;
    QList<int> mBulletID;
    bool mIs3D;
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mCombatModelNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mAttackNodeLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mOperatorNode {nullptr};
    bool mOperatorIsAttacker{false};
};

#endif // COMBATMODELNODE_H
