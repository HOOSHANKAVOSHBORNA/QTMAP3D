#ifndef BULLETNODE_H
#define BULLETNODE_H

#include "flyableModelNode.h"
#include "explosion.h"

class BulletNode : public FlyableModelNode
{
public:
    BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkProxyModel *bookmark);
    void attackTo(osgEarth::GeoPoint position);
    void explode();
    osgEarth::GeoPoint getTargetPosition();



private:
    Explosion *mExplosion;
    MapItem *mMapItem;
    std::string mBulletModelURL;
    std::string mBulletIconURL;
    QQmlEngine *mEngine;
    BookmarkProxyModel *mBookmark;
    osgEarth::GeoPoint mTarget;
};

#endif // BULLETNODE_H
