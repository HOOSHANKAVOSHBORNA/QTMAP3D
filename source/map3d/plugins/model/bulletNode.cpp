#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark):
    FlyableModelNode(mapControler, modelUrl, iconUrl, engine, bookmark)
{
    mMapItem = mapControler;
}

void BulletNode::attackTo(osgEarth::GeoPoint position)
{
    this->flyTo(position,1500);
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

void BulletNode::setTargetPosition(osgEarth::GeoPoint geoPos)
{
    mTarget = geoPos;
}


