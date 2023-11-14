#ifndef ATTACKMANAGER_H
#define ATTACKMANAGER_H

#include "bulletNode.h"

class AttackManager
{
public:
    AttackManager(MapItem *mapControler , SimpleModelNode *parent);
    void attackResult(bool result ,int bulletID );
    int readyBullet(const std::string url3D , const std::string url2D);
    void attackTo(int bulletID ,osgEarth::GeoPoint geoPos);
    void removeBullet(int bulletID);
    void setBulletTargetModel(int bulletID ,SimpleModelNode *model);
    void setBulletCount(const int count);
    int getBulletCount();
    osgEarth::GeoPoint getBulletPosition(int bulletID);
    SimpleModelNode* getBulletTargetModel(int bulletID);
    void setAttackLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getAttackLayer();
    osg::ref_ptr<BulletNode> getBulletNode(int bulletID);
    QList<SimpleModelNode*> getNearTargets();
    void setNearTargets(SimpleModelNode* targetNode);
    void showNearTargets();
    void hideNearTargets();
private:
    osg::ref_ptr<ParenticAnnotationLayer> mAttackManagerLayer;
    QList<osg::ref_ptr<BulletNode>> mBulletList;
    MapItem *mMapItem;
    SimpleModelNode *mParent;
    int mBulletCount = 0;
    QList<SimpleModelNode*> mNearTargets;
    osg::ref_ptr<osg::Group> mHighlightGroup;


};

#endif // ATTACKMANAGER_H
