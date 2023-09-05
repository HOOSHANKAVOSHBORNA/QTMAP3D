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
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mMapItem, &MapItem::modeChanged, this, &SimpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();
    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;
    osg::ref_ptr<osg::Node> simpleNode = osgDB::readRefNodeFile(modelUrl);
    m3DNode->addChild(simpleNode, 0, std::numeric_limits<float>::max());
    //--root ------------------------------------------------------------
    mSwitchNode = new osg::Switch;
//    osg::ref_ptr<osg::Material> mat = new osg::Material;
//    mat->setDiffuse (osg::Material::FRONT_AND_BACK, osgEarth::Color::Silver);
    //mat->setAmbient(osg::Material::FRONT_AND_BACK, osgEarth::Color::Silver);
    //mat->setColorMode(osg::Material::AMBIENT);
    //getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

//    mHighlight = new osgFX::Scribe;
//    mHighlight->setWireframeColor(osg::Vec4f(1,0,0,1));
    //osgEarth::Registry::shaderGenerator().run(mOutline);
//    mOutline->setWidth(10);
//    osg::ref_ptr<osg::StateSet> vOutlineState = mOutline->getOrCreateStateSet();
    //vOutlineState->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
    //mOutline->getOrCreateStateSet()->setMode(GL_LIGHTING,
    //osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //mOutline->getOrCreateStateSet()->setAssociatedModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //mOutline->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //mOutline->setColor(osg::Vec4f(1,0,0,1));

    //mOutline->dirtyBound();
    //osg::DisplaySettings::instance()->setMinimumNumStencilBits(1);
    //osg::DisplaySettings::instance()->setRGB(true);
    //osg::DisplaySettings::instance()->setDepthBuffer(true);
    //mapItem()->getViewer()->getCamera()->setClearColor(osgEarth::Color::Blue);
    //mapItem()->getViewer()->getCamera()->setClearStencil(1);
    //mapItem()->getViewer()->setLightingMode(OSGRenderNode::LightingMode::HEADLIGHT);
    //osgEarth::Symbology::Style  rootStylee;
    //rootStylee.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(m3DNode);
    //rootStylee.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Blue;
    //rootStylee.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color::Blue;
    //setStyle(rootStylee);
    //mapItem()->getViewer()->getCamera()->setClearMask(
    //    GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT
    //    );
    //mapItem()->getViewer()->getCamera()->setClearStencil(0);
    //osg::ref_ptr<osg::CullFace> cf = new osg::CullFace;
    //cf->setMode(osg::CullFace::FRONT);
    //vOutlineState->setAttributeAndModes(cf, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    //mapItem()->getViewer()->getCamera()->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

    setCullingActive(false);

    double modelLenght = simpleNode->getBound().radius() * 2;
    qDebug()<<"modelLenght: "<<modelLenght;
    int ScaleRatio;
    if (3<modelLenght && modelLenght<7){
        ScaleRatio = 20;
    }

    else if(modelLenght<7 && modelLenght<15){
        ScaleRatio = 4;
    }

    else {
        ScaleRatio = 2;
    }
    mScaler = new ModelAutoScaler(ScaleRatio, 1, 1000);
    if (mScalability){
        setCullCallback(mScaler);
    }


    //--2D node---------------------------------------------------------
    m2DNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(iconUrl);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(image, osg::Vec2s(0,0), 0, 0, 0.2);
    m2DNode->setStateSet(geodeStateSet);
    m2DNode->addDrawable(imgGeom);
    osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
    pm->setMode(osg::PolygonMode::FRONT_AND_BACK,
                osg::PolygonMode::LINE);
    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchNode->addChild(m3DNode, true);
        mSwitchNode->addChild(m2DNode, false);
    }
    else{
        mSwitchNode->addChild(m3DNode, false);
        mSwitchNode->addChild(m2DNode, true);
    }


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



    //-------------------------------------------------------------------------
    double boundRadius = m3DNode->computeBound().radius();
    //---circle indicator------------------------------------------------------
    mCircle = new Circle();
    mCircle->setFillColor(osg::Vec4f(0,0.6,0.6,0));
    mCircle->setStrokeColor(osg::Vec4f(0,1,0,1));
    mCircle->setStrokeWidth(2);
    mCircle->setRadius(osgEarth::Distance(boundRadius, osgEarth::Units::METERS));
    mCircle->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.5));
    mCircle->setNodeMask(0);
    //---coneIndicator----------------------------------------------------------
    mCone = new Cone();
    mCone->setFillColor(osg::Vec4f(0,1,0,0.4));
    mCone->setRadius(osgEarth::Distance(boundRadius/4, osgEarth::Units::METERS));
    mCone->setHeight(osgEarth::Distance(boundRadius/2, osgEarth::Units::METERS));
    mCone->setLocalRotation(osg::Quat(osg::PI,osg::Vec3d(1,1,0)));
    mCone->setCenter(osg::Vec3d(0,0,-boundRadius*2));
    mCone->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0));
    mCone->setNodeMask(0);
    //--------------------------------------------------------------------------

    root = new osg::Group;
    root->addChild(mSwitchNode);
    root->addChild(mCone);
    root->addChild(mCircle);
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(root);
    rootStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Blue;
    rootStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color::Blue;
    setStyle(rootStyle);
}

void SimpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        if(mIsSelected){
            mCone->setNodeMask(1);
        }
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mCone->setNodeMask(0);
    }
}

bool SimpleModelNode::getScalability() const
{
    return mScalability;
}

void SimpleModelNode::setScalability(bool newScalability)
{
    mScalability = newScalability;
    if (mScalability){
        setCullCallback(mScaler);
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
        mCircle->setNodeMask(1);
        if(mIs3D){
        mCone->setNodeMask(1);
        }else{
        mCone->setNodeMask(0);
        }
    }else{
        mCircle->setNodeMask(0);
        mCone->setNodeMask(0);
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
