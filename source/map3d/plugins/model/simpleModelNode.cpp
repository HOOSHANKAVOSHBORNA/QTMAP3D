#include "simpleModelNode.h"
#include "model.h"
#include "modelAutoScaler.h"
#include "mapItem.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>

simpleModelNode::simpleModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent)
    : QObject{parent},
      osgEarth::Annotation::ModelNode(mapControler->getMapNode(), model::getDefaultStyle()),
    mModelUrl(modelUrl),
    mMapItem(mapControler),
    mIconUrl(iconUrl)

{
    osg::ref_ptr<osg::Node> simpleNode = osgDB::readRefNodeFile(modelUrl);
    setCullingActive(false);
    addCullCallback(new ModelAutoScaler(2.5, 1, 1000));

    connect(mMapItem, &MapItem::modeChanged, this, &simpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    m2DImage = osgDB::readImageFile(mIconUrl);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(m2DImage, osg::Vec2s(0,0), 0, 0, 0.2);
    mImgGeod = new osg::Geode();
    mImgGeod->setStateSet(geodeStateSet);
    mImgGeod->addDrawable(imgGeom);
    mMode = new osg::Switch;
    mMode->addChild(simpleNode, true);
    mMode->addChild(mImgGeod, false);

    osgEarth::Symbology::Style  modelStyle;
    modelStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mMode);
    setStyle(modelStyle);       
}

simpleModelNode *simpleModelNode::getNewModel()
{
    return new simpleModelNode(mMapItem, mModelUrl, mIconUrl);
}

osg::ref_ptr<osg::Image> simpleModelNode::getM2DImage() const
{
    return m2DImage;
}

void simpleModelNode::setM2DImage(const osg::ref_ptr<osg::Image> &newM2DImage)
{
    m2DImage = newM2DImage;
}

void simpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D)
    {
        mMode->setValue(0,true);
        mMode->setValue(1, false);
    }
    else
    {
        mMode->setValue(0, false);
        mMode->setValue(1,true);
    }
}
