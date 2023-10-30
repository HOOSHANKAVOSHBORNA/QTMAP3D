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
#include <mainwindow.h>
#include "attackManager.h"

const float RANGE3D = 835;
QMap<std::string, osg::ref_ptr<osg::Node>> SimpleModelNode::mNodes3D;
QMap<std::string, osg::ref_ptr<osg::Image>> SimpleModelNode::mImages2D;


SimpleModelNode::SimpleModelNode(MapItem *mapControler, const std::string &url3D, const std::string &url2D, QQmlEngine *engine, BookmarkManager *bookmark, QObject *parent)
    : QObject{parent},
    osgEarth::Annotation::ModelNode(mapControler->getMapNode(), Model::getDefaultStyle()),
    mUrl3D(url3D),
    mMapItem(mapControler),
    mUrl2D(url2D),
    mEnigine(engine),
    mBookmark(bookmark)
{
    connect(mMapItem, &MapItem::modeChanged, this, &SimpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();

    compile();
}

SimpleModelNode::~SimpleModelNode()
{
    delete mNodeInformation;
}

void SimpleModelNode::updateUrl(const std::string &url3D, const std::string &url2D)
{
    if(mUrl2D != url2D || mUrl3D != url3D){
        mUrl2D = url2D;
        mUrl3D = url3D;
        compile();
    }

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

bool SimpleModelNode::getIsBookmarked() const
{
    return mIsBookmarked;
}

void SimpleModelNode::setIsBookmarked(bool newIsBookmarked)
{
    mIsBookmarked = newIsBookmarked;
}

void SimpleModelNode::isAttacker(ParenticAnnotationLayer *layer, int bulletCount)
{
    mAttackManager = new AttackManager(mMapItem,mEnigine,mBookmark,this);
    mAttackManager->setAttackLayer(layer);
    mAttackManager->setBulletCount(bulletCount);
}

AttackManager *SimpleModelNode::getAttackManager()
{
    return mAttackManager;
}

NodeData *SimpleModelNode::nodeData() const
{
    return mNodeData;
}

void SimpleModelNode::setNodeData(NodeData *newNodeData)
{
    mNodeData = newNodeData;
    updateUrl(mNodeData->url3D, mNodeData->url2D);
    setModelColor(osgEarth::Color(mNodeData->color));
    if (mNodeInformation)
        mNodeInformation->addUpdateNodeInformationItem(newNodeData);
}

void SimpleModelNode::setModelColor(osgEarth::Color color)
{
    if(mColor != color){
        //--recolor 3D Node----------------------------------------------------
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse (osg::Material::FRONT_AND_BACK, color);
        mSimpleNode->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
        //--recolor 2D Node----------------------------------------------------
        osg::Vec4 imageColor = color;
        for(int i=0; i<mImage->s(); ++i) {
            for(int j=0; j<mImage->t(); ++j) {
                imageColor.a() = mImage->getColor(i, j).a();
                mImage->setColor(imageColor, i, j);
            }
        }
        mImage->dirty();
        //---------------------------------------------------------------------
    }
    mColor = color;

}

void SimpleModelNode::compile()
{
    //--read node and image----------------------------------------------
    if (mImages2D.contains(mUrl2D)){
        mImage = mImages2D[mUrl2D];
    }
    else {
        mImage = osgDB::readImageFile(mUrl2D);
        mImages2D[mUrl2D] = mImage ;
    }
    if (mNodes3D.contains(mUrl3D)){
        mSimpleNode = mNodes3D[mUrl3D];
    }
    else {
        mSimpleNode = osgDB::readRefNodeFile(mUrl3D);
        mNodes3D[mUrl3D] = mSimpleNode ;
    }
    //--auto scale------selectModel----------------------------------------------
    double modelLenght = mSimpleNode->getBound().radius() * 2;
//    qDebug()<<"len: "<<modelLenght;
    double scaleRatio = 100/modelLenght;
    double iconSize = 400/ scaleRatio;
    //    if (modelLenght < 12){
    //        scaleRatio = 14;
    //        mImage->scaleImage(28, 28, mImage->r());
    //    }
    //    else if (modelLenght < 24){
    //        scaleRatio = 5;
    //        mImage->scaleImage(78, 78, mImage->r());
    //    }
    //    else if (modelLenght < 36){
    //        scaleRatio = 10;
    //        mImage->scaleImage(39, 39, mImage->r());
    //    }
    //    else if (modelLenght < 48){
    //        scaleRatio = 8;
    //        mImage->scaleImage(49, 49, mImage->r());
    //    }
    //    else if (modelLenght < 60){
    //        scaleRatio = 6;
    //        mImage->scaleImage(65, 65, mImage->r());
    //    }
    //    else if (modelLenght < 72){
    //        scaleRatio = 4;
    //        mImage->scaleImage(98, 98, mImage->r());
    //    }
    //    else if (modelLenght < 84){
    //        scaleRatio = 2;
    //        mImage->scaleImage(196, 196, mImage->r());
    //    }
    //    else{
    //        scaleRatio = 1;
    //        mImage->scaleImage(392, 392, mImage->r());
    //    }
    mImage->scaleImage(iconSize, iconSize, mImage->r());
    setCullingActive(false);
    mAutoScaler = new ModelAutoScaler(scaleRatio, 1, 1000);
    if (mIsAutoScale){
        setCullCallback(mAutoScaler);
    }
    //--root node--------------------------------------------------------
    mSwitchNode = new osg::Switch;
    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;

    m3DNode->addChild(mSimpleNode, 0, std::numeric_limits<float>::max());
    //--2D node---------------------------------------------------------
    m2DNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImage, osg::Vec2s(0,0), 0, 0, 0.2);
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
    //--------------------------------------------------------------------------
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mSwitchNode);
    //rootStyle.getOrCreate<osgEarth::Symbology::Color(osgEarth::Color::Aqua)>();
    setStyle(rootStyle);
    setModelColor(mColor);
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

void SimpleModelNode::selectModel()
{
    if (!mNodeInformation){
        mNodeInformation = new NodeInformation(mEnigine, this);
        connect(mNodeInformation, &NodeInformation::bookmarkChecked, [&](bool t){
            mIsBookmarked = t;
            if (mIsBookmarked){
                mBookmarkItem = new BookmarkItem(QString::fromStdString(mNodeData->type), QString::fromStdString(mNodeData->name),mNodeInformation->wnd() , QString::fromStdString(mNodeData->iconSrc));
                mBookmark->addBookmarkItem(mBookmarkItem);
            }
            else{
                mBookmark->removeBookmarkItem(mBookmarkItem);
                delete mBookmarkItem;
            }
        });
        mNodeInformation->addUpdateNodeInformationItem(mNodeData);
    }
    mNodeInformation->show();
    mIsSelected = !mIsSelected;
    if(mIsSelected){
        mSwitchNode->setValue(2, true);
    }else {
        mSwitchNode->setValue(2, false);
    }
}

std::string SimpleModelNode::url3D() const
{
    return mUrl3D;
}

std::string SimpleModelNode::url2D() const
{
    return mUrl2D;
}

MapItem *SimpleModelNode::mapItem() const
{
    return mMapItem;
}
