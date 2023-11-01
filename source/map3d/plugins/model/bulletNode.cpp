#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl):
    FlyableModelNode(mapControler, modelUrl, iconUrl)
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


