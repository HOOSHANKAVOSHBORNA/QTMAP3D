#ifndef BULLETNODE_H
#define BULLETNODE_H

#include "flyableModelNode.h"
#include "explosion.h"

class BulletNode : public FlyableModelNode
{
public:
    BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark);
    void attackTo(osgEarth::GeoPoint position);
    Explosion* explode();
    osgEarth::GeoPoint getTargetPosition();
    void setTargetPosition(osgEarth::GeoPoint geoPos);
    osgEarth::Annotation::ModelNode *getDragModelNode();
private:
    osg::ref_ptr<Explosion> mExplosion;
    MapItem *mMapItem;
    std::string mBulletModelURL;
    std::string mBulletIconURL;
    QQmlEngine *mEngine;
    BookmarkManager *mBookmark;
    osgEarth::GeoPoint mTarget;
};

#endif // BULLETNODE_H
