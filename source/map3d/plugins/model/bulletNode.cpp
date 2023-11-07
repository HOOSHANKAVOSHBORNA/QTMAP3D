#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl):
    FlyableModelNode(mapControler, modelUrl, iconUrl),
    mMapItem(mapControler),
    mBulletModelURL(modelUrl),
    mBulletIconURL(iconUrl)
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

SimpleModelNode *BulletNode::getTargetModel()
{
    return mTargetModel;
}

void BulletNode::setTargetModel(SimpleModelNode *model)
{
    mTargetModel = model;
}

osgEarth::Annotation::ModelNode *BulletNode::getDragModelNode()
{
    osgEarth::Symbology::Style  style = getStyle();
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, osgEarth::Color::Gray);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> dragModelNode = new osgEarth::Annotation::ModelNode(getMapNode(), style);
    dragModelNode->setCullingActive(false);
    dragModelNode->addCullCallback(getCullCallback());
    dragModelNode->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    return dragModelNode.release();
}
