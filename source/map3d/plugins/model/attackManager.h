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
    void setTargetPosition(int bulletID ,osgEarth::GeoPoint geoPos);
    void setBulletCount(const int count);
    int getBulletCount();
    osgEarth::GeoPoint getBulletPosition(int bulletID);
    osgEarth::GeoPoint getTargetPosition(int bulletID);
    void setAttackLayer(ParenticAnnotationLayer* layer);
    ParenticAnnotationLayer *getAttackLayer();


private:
    osg::ref_ptr<ParenticAnnotationLayer> mAttackManagerLayer;
    QList<osg::ref_ptr<BulletNode>> mBulletList;
    MapItem *mMapItem;
    QQmlEngine *mEngine;
    BookmarkManager *mBookmark;
    SimpleModelNode *mParent;
    int mBulletCount = 0;


};

#endif // ATTACKMANAGER_H
