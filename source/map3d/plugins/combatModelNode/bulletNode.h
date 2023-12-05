#ifndef BULLETNODE_H
#define BULLETNODE_H

#include "flyableModelNode.h"
#include "explosion.h"

class BulletNode : public FlyableModelNode
{
public:
    BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl);
    void attackTo(osgEarth::GeoPoint position);
    Explosion* explode();
    SimpleModelNode* getTargetModel();
    void setTargetModel(SimpleModelNode *model);
private:
    osg::ref_ptr<Explosion> mExplosion;
    MapItem *mMapItem;
    QQmlEngine *mEngine;
    BookmarkManager *mBookmark;
    osg::ref_ptr<SimpleModelNode> mTargetModel;
};

#endif // BULLETNODE_H