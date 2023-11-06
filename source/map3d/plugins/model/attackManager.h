#ifndef ATTACKMANAGER_H
#define ATTACKMANAGER_H

#include "bulletNode.h"

class AttackManager
{
public:
    AttackManager(MapItem *mapControler, QQmlEngine *engine, BookmarkManager *bookmark , SimpleModelNode *parent = nullptr);
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
    int setNearTargets(SimpleModelNode* targetNode);


private:
    osg::ref_ptr<ParenticAnnotationLayer> mAttackManagerLayer;
    QList<osg::ref_ptr<BulletNode>> mBulletList;
    MapItem *mMapItem;
    QQmlEngine *mEngine;
    BookmarkManager *mBookmark;
    SimpleModelNode *mParent;
    int mBulletCount = 0;
    QList<SimpleModelNode*> mNearTargets;


};

#endif // ATTACKMANAGER_H
