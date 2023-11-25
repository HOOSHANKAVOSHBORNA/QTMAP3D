#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H


#include "bulletNode.h"
#include "lineNode.h"
#include <simpleModelNode.h>

struct assignmentData
{
public:
    osg::ref_ptr<SimpleModelNode> attacker{nullptr};
    osg::ref_ptr<SimpleModelNode> target{nullptr};
    osg::ref_ptr<LineNode> relationLine{nullptr};
};

class CombatManager
{
public:
    CombatManager(MapItem *map);
    //--setupFunctions-----------------------------------------------------------------------
    void setCombatLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getCombatLayer();
    //--dataManagementFunctions--------------------------------------------------------------
    void assign(SimpleModelNode *attacker, SimpleModelNode *target, MapItem *map);
    void removeAssignment(SimpleModelNode *attacker,SimpleModelNode *target);
    void deleteAttackerNode(SimpleModelNode *attacker);
    void deleteTargetNode(SimpleModelNode *target);
    QList<assignmentData> *getAssignmentData();
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
    QList<assignmentData> *mEdgeDataList;
};

#endif // COMBATMANAGER_H
