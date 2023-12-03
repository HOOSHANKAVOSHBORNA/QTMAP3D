#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H


#include "bulletNode.h"
#include "lineNode.h"
#include "assignLine.h"
#include <simpleModelNode.h>


 enum State{
    PREASSIGN,
    HOVERED,
    SELECTED,
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
    osg::ref_ptr<LineNode> relationLine{nullptr};
    State state{PREASSIGN};
};



class CombatManager
{
public:


    CombatManager(MapItem *map);
    //--setupFunctions-----------------------------------------------------------------------
    void setCombatLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getCombatLayer();
    //--dataManagementFunctions--------------------------------------------------------------
    void assign(SimpleModelNode *attacker, SimpleModelNode *target, State state=PREASSIGN);
    void removeAssignment(SimpleModelNode *attacker,SimpleModelNode *target);
    void deleteAttackerNode(SimpleModelNode *attacker);
    void deleteTargetNode(SimpleModelNode *target);
    void setState(assignmentData data, State state);
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
