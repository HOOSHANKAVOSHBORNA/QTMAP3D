#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "bulletNode.h"
#include <simpleModelNode.h>

class AssignLine;

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

struct assignmentData
{
public:
    osg::ref_ptr<SimpleModelNode> attacker{nullptr};
    osg::ref_ptr<SimpleModelNode> target{nullptr};
    void setState(AssignState state) const;
    void setLine(AssignState state,osgEarth::GeoPoint start , osgEarth::GeoPoint end, MapItem *map = nullptr);
    AssignLine *getLine();
    AssignState getState() const;

private:
    AssignLine *mRelationLine{nullptr};
    AssignState mState{PREASSIGN};
};



class CombatManager
{
public:
    CombatManager(MapItem *map);
    //--setupFunctions-----------------------------------------------------------------------
    void setCombatLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getCombatLayer();
    //--dataManagementFunctions--------------------------------------------------------------
    void assign(SimpleModelNode *attacker, SimpleModelNode *target, AssignState state=PREASSIGN);
    void removeAssignment(SimpleModelNode *attacker,SimpleModelNode *target);
    void deleteAttackerNode(SimpleModelNode *attacker);
    void deleteTargetNode(SimpleModelNode *target);
    QMap<QString,assignmentData> *getAssignmentData();
    //--bulletManagementFunctions----------------------------------------------------------------------
    int readyBulletFor(SimpleModelNode *attacker,const std::string &url3D , const std::string &url2D);
    void removeBullet(int bulletID);
    osg::ref_ptr<BulletNode> getBulletNode(int bulletID);
    void setBulletTargetModel(int bulletID ,SimpleModelNode *target);
    SimpleModelNode* getBulletTargetModel(int bulletID);
    void attackTo(int bulletID ,osgEarth::GeoPoint geoPos);
    osgEarth::GeoPoint getBulletPosition(int bulletID);
    void attackResult(bool result ,int bulletID );

private:
    MapItem *mMapItem{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mCombatLayer;
    QList<osg::ref_ptr<BulletNode>> *mBulletList;
    // QList<assignmentData> *mEdgeDataList;
    QMap<QString,assignmentData> *mAssignmentDataMap;
};

#endif // COMBATMANAGER_H
