#include "simpleModelNode.h"
#include "model.h"
#include "modelAutoScaler.h"
#include "mapItem.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
const float RANGE3D = 835;
SimpleModelNode::SimpleModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent)
    : QObject{parent},
      osgEarth::Annotation::ModelNode(mapControler->getMapNode(), Model::getDefaultStyle()),
    mModelUrl(modelUrl),
    mMapItem(mapControler),
    mIconUrl(iconUrl)

{
    connect(mMapItem, &MapItem::modeChanged, this, &SimpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();

    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;
    osg::ref_ptr<osg::Node> simpleNode = osgDB::readRefNodeFile(modelUrl);
    m3DNode->addChild(simpleNode, 0, std::numeric_limits<float>::max());
    //--root ------------------------------------------------------------
    mSwitchNode = new osg::Switch;
    setCullingActive(false);


    double modelLenght = simpleNode->getBound().radius() * 2;
    qDebug()<<"modelLenght: "<<modelLenght;
    int ScaleRatio;
    if (3<modelLenght && modelLenght<7){
        ScaleRatio = 20;
    }

    else if(modelLenght<10 && modelLenght<15){
        ScaleRatio = 4;
    }

    else {
        ScaleRatio = 2;
    }
    if (scalability){
        addCullCallback(new ModelAutoScaler(ScaleRatio, 1, 1000));
    }


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

SimpleModelNode *SimpleModelNode::getNewModel()
{
    return new SimpleModelNode(mMapItem, mModelUrl, mIconUrl);
}

void SimpleModelNode::onModeChanged(bool is3DView)
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

bool SimpleModelNode::getScalability() const
{
    return scalability;
}

void SimpleModelNode::setScalability(bool newScalability)
{
    scalability = newScalability;
}

std::string SimpleModelNode::modelUrl() const
{
    return mModelUrl;
}

std::string SimpleModelNode::iconUrl() const
{
    return mIconUrl;
}

MapItem *SimpleModelNode::mapItem() const
{
    return mMapItem;
}
