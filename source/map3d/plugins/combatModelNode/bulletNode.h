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
    osgEarth::Annotation::ModelNode *getDragModelNode();
private:
    osg::ref_ptr<Explosion> mExplosion;
    MapItem *mMapItem;
    std::string mBulletModelURL;
    std::string mBulletIconURL;
    QQmlEngine *mEngine;
    BookmarkManager *mBookmark;
    osg::ref_ptr<SimpleModelNode> mTargetModel;
};

#endif // BULLETNODE_H
