#include "simpleModelNode.h"
#include "model.h"
#include <osg/CullFace>
#include <osg/PolygonMode>
#include "mapItem.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osgEarthSymbology/ModelSymbol>
#include <osgEarth/GLUtils>
#include <osgEarth/Registry>
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
    //--root node--------------------------------------------------------
    mSwitchNode = new osg::Switch;
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
    //--select node---------------------------------------------------
    osg::ref_ptr<osg::Group> selectGroup = new osg::Group;
    osg::ComputeBoundsVisitor cbv;
    if(mIs3D)
        m3DNode->accept(cbv);
    else
        m2DNode->accept(cbv);
    mCircleSelectNode = new Circle();
    mCircleSelectNode->setFillColor(osg::Vec4f(0,0.0,0.0,0));
    mCircleSelectNode->setStrokeColor(osg::Vec4f(0,1,0,0.5));
    mCircleSelectNode->setStrokeWidth(2);
    mCircleSelectNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    mCircleSelectNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.5));

    mConeSelecteNode = new Cone();
    mConeSelecteNode->setFillColor(osg::Vec4f(0,1,0,0.2));
    mConeSelecteNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius()/4, osgEarth::Units::METERS));
    mConeSelecteNode->setHeight(osgEarth::Distance(cbv.getBoundingBox().radius()/2, osgEarth::Units::METERS));
    mConeSelecteNode->setLocalRotation(osg::Quat(osg::PI,osg::Vec3d(1,1,0)));
    mConeSelecteNode->setCenter(osg::Vec3d(0,0,-mConeSelecteNode->getHeight().as(osgEarth::Units::METERS)/2));
    mConeSelecteNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,cbv.getBoundingBox().zMax()));

    selectGroup->addChild(mCircleSelectNode);
    selectGroup->addChild(mConeSelecteNode);
    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchNode->addChild(m3DNode, true);
        mSwitchNode->addChild(m2DNode, false);
        mSwitchNode->addChild(selectGroup, false);
    }
    else{
        mSwitchNode->addChild(m3DNode, false);
        mSwitchNode->addChild(m2DNode, true);
        mSwitchNode->addChild(selectGroup, false);
    }
    //--auto scale----------------------------------------------------
    setCullingActive(false);
    double modelLenght = simpleNode->getBound().radius() * 2;
    double scaleRatio;
    if (3 < modelLenght && modelLenght < 7)
        scaleRatio = 20;
    else if(modelLenght < 7 && modelLenght < 15)
        scaleRatio = 4;
    else
        scaleRatio = 1.3;

    mAutoScaler = new ModelAutoScaler(scaleRatio, 1, 1000);
    if (mIsAutoScale){
        setCullCallback(mAutoScaler);
    }
    //--------------------------------------------------------------------------
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mSwitchNode);
    setStyle(rootStyle);
}

void SimpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    osg::ComputeBoundsVisitor cbv;

    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);

        m3DNode->accept(cbv);
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);

        m2DNode->accept(cbv);
    }

    mCircleSelectNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    mConeSelecteNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius()/4, osgEarth::Units::METERS));
    mConeSelecteNode->setHeight(osgEarth::Distance(cbv.getBoundingBox().radius()/2, osgEarth::Units::METERS));
    mConeSelecteNode->setCenter(osg::Vec3d(0,0,-mConeSelecteNode->getHeight().as(osgEarth::Units::METERS)/2));
    mConeSelecteNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,cbv.getBoundingBox().zMax()));
}

bool SimpleModelNode::isAutoScale() const
{
    return mIsAutoScale;
}

void SimpleModelNode::setAutoScale(bool newIsAutoScale)
{
    mIsAutoScale = newIsAutoScale;
    if (mIsAutoScale){
        setCullCallback(mAutoScaler);
    }
    else{
        setCullCallback(nullptr);
        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
    }
}

void SimpleModelNode::selectModel(bool isSelected)
{
    mIsSelected = isSelected;
    if(isSelected){
        mSwitchNode->setValue(2, true);
    }else{
        mSwitchNode->setValue(2, false);
    }
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
