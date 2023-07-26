#include "simpleModelNode.h"
#include "model.h"
#include "modelAutoScaler.h"
#include "mapItem.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
const float RANGE3D = 835;
simpleModelNode::simpleModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent)
    : QObject{parent},
      osgEarth::Annotation::ModelNode(mapControler->getMapNode(), model::getDefaultStyle()),
    mModelUrl(modelUrl),
    mMapItem(mapControler),
    mIconUrl(iconUrl)

{
    connect(mMapItem, &MapItem::modeChanged, this, &simpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();

    //--root ------------------------------------------------------------
    mSwitchNode = new osg::Switch;
    setCullingActive(false);
    addCullCallback(new ModelAutoScaler(2.5, 1, 1000));

    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;
    osg::ref_ptr<osg::Node> simpleNode = osgDB::readRefNodeFile(modelUrl);
    m3DNode->addChild(simpleNode, 0, std::numeric_limits<float>::max());

    //--2D node---------------------------------------------------------
    m2DNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(iconUrl);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(image, osg::Vec2s(0,0), 0, 0, 0.2);
    m2DNode->setStateSet(geodeStateSet);
    m2DNode->addDrawable(imgGeom);

    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchNode->addChild(m3DNode, true);
        mSwitchNode->addChild(m2DNode, false);
    }
    else{
        mSwitchNode->addChild(m3DNode, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mSwitchNode);
    setStyle(rootStyle);
//    osg::AutoTransform *at = new osg::AutoTransform;
//    //at->addChild(mMode);
//    at->addChild(mImgGeod);
//    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);
//    if(mIs3D)
//    {
//        mBaseNode->addChild(simpleNode, 0, RANGE3D);
//        mBaseNode->addChild(at, RANGE3D, std::numeric_limits<float>::max());
//    }
//    else
//    {
//        mBaseNode->addChild(simpleNode, 0, 0);
//        mBaseNode->addChild(at, 0, std::numeric_limits<float>::max());
//    }
}

simpleModelNode *simpleModelNode::getNewModel()
{
    return new simpleModelNode(mMapItem, mModelUrl, mIconUrl);
}

void simpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
    }
}

std::string simpleModelNode::modelUrl() const
{
    return mModelUrl;
}

std::string simpleModelNode::iconUrl() const
{
    return mIconUrl;
}

MapItem *simpleModelNode::mapItem() const
{
    return mMapItem;
}
