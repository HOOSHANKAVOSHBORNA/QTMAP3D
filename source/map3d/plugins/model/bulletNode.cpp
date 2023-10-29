#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark):
    FlyableModelNode(mapControler, modelUrl, iconUrl, engine, bookmark)
{
}

void BulletNode::attackTo(osgEarth::GeoPoint position)
{
    this->flyTo(position,100);
}

void BulletNode::explode()
{
    mExplosion = new Explosion(mMapItem);
    mExplosion->setMapNode(mMapItem->getMapNode());
    this->addChild(mExplosion);
}


