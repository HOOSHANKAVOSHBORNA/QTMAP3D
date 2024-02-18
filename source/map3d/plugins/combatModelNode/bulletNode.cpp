#include "bulletNode.h"

BulletNode::BulletNode(QQmlEngine *engine,
                       MapItem *mapControler,
                       const std::string &modelUrl,
                       const std::string &iconUrl)
    : FlyableModelNode(engine, mapControler, modelUrl, iconUrl)
    , mMapItem(mapControler)
{
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

SimpleModelNode *BulletNode::getTargetModel()
{
    return mTargetModel;
}

void BulletNode::setTargetModel(SimpleModelNode *model)
{
    mTargetModel = model;
}
