#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark):
    FlyableModelNode(mapControler, modelUrl, iconUrl, engine, bookmark)
{
    mMapItem = mapControler;
}

void BulletNode::attackTo(osgEarth::GeoPoint position)
{
    mTarget = position;
    this->flyTo(position,100);
}

Explosion *BulletNode::explode()
{
    mExplosion = new Explosion(mMapItem);
    mExplosion->setMapNode(mMapItem->getMapNode());
    return mExplosion;
}

osgEarth::GeoPoint BulletNode::getTargetPosition()
{
    return mTarget;
}


