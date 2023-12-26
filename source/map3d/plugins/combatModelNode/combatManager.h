#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "bulletNode.h"
#include <simpleModelNode.h>
#include <QVector>
#include "assignLine.h"

 enum AssignState{
    PREASSIGN,
    ASSIGNED,
    BUSY,
    SEARCH,
    LOCK,
    FIRE,
    SUCCEED,
    FAILED
};

struct Assignment
{
public:
    Assignment(MapItem *mapItem, SimpleModelNode *attacker, SimpleModelNode *target);
    void setState(AssignState state);
    QColor getColor();

    osg::ref_ptr<SimpleModelNode> attacker{nullptr};
    osg::ref_ptr<SimpleModelNode> target{nullptr};

    osg::ref_ptr<AssignLine> assignLine{nullptr};
    AssignState state{PREASSIGN};
};

class CombatManager: public QObject
{
    Q_OBJECT

public:
    CombatManager(MapItem *map);
    //--setupFunctions-----------------------------------------------------------------------
    void setCombatLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getCombatLayer();
    //--dataManagementFunctions--------------------------------------------------------------
    void assign(SimpleModelNode *attacker, SimpleModelNode *target, AssignState state=PREASSIGN);
    void removeAssignment(SimpleModelNode *attacker,SimpleModelNode *target);
    void removeAttackerAssignments(SimpleModelNode *attacker);
    void removeTargetAssignments(SimpleModelNode *target);
    //--bulletManagementFunctions----------------------------------------------------------------------
    int readyBulletFor(SimpleModelNode *attacker,const std::string &url3D , const std::string &url2D);
    void removeBullet(int bulletID);
    osg::ref_ptr<BulletNode> getBulletNode(int bulletID);
    void setBulletTargetModel(int bulletID ,SimpleModelNode *target);
    SimpleModelNode* getBulletTargetModel(int bulletID);
    void attackTo(int bulletID ,osgEarth::GeoPoint geoPos);
    osgEarth::GeoPoint getBulletPosition(int bulletID);
    void attackResult(bool result ,int bulletID );
    //-----------------------------------------------------

    Assignment* getAssignment(SimpleModelNode *attacker, SimpleModelNode *target);
    QList<Assignment*> getAttackerAssignments(SimpleModelNode *attacker);
    QList<Assignment*> getTargetAssignments(SimpleModelNode *target);

signals:
    void dataChanged();

private:
    MapItem *mMapItem{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mCombatLayer;
    QList<osg::ref_ptr<BulletNode>> *mBulletList;
    QList<Assignment*> mAssignmentList;
};

#endif // COMBATMANAGER_H
