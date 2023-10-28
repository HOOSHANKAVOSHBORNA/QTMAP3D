#ifndef ATTACKERMODELNODE_H
#define ATTACKERMODELNODE_H


#include "mainwindow.h"
#include "simpleModelNode.h"
#include "flyableModelNode.h"
#include "explosion.h"

class AttackerModelNode : public SimpleModelNode
{
public:
    AttackerModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkProxyModel *bookmark);
    AttackerModelNode* asAttackerModelNode() override {return this;}
    void ready(int count);
    void attackTo(osgEarth::GeoPoint position);
    void attackResult(bool result, osgEarth::GeoPoint position);



private:
    FlyableModelNode *mBulletNode{nullptr};
    Explosion *mExplosion;
    osgEarth::GeoPoint mLastPosition;
    MapItem *mMapItem;
    std::string mBulletModelURL;
    std::string mBulletIconURL;
    QQmlEngine *mEngine;
    BookmarkProxyModel *mBookmark;
};

#endif // ATTACKERMODELNODE_H
