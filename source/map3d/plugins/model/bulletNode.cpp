#include "bulletNode.h"

BulletNode::BulletNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark):
    FlyableModelNode(mapControler, modelUrl, iconUrl, engine, bookmark),
    mMapItem(mapControler),
    mBulletModelURL(modelUrl),
    mBulletIconURL(iconUrl),
    mEngine(engine),
    mBookmark(bookmark)
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
