#include "simpleModelNode.h"
#include <osg/CullFace>
#include <osg/PolygonMode>
#include "mapItem.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthSymbology/ModelSymbol>
#include <osgEarth/GLUtils>
#include <osgEarth/Registry>
#include <mainwindow.h>
#include <QtQml>
#include <qmlNode.h>
#include <utility.h>
#include <osg/Stencil>

const float RANGE3D = 835;


QMap<std::string, osg::ref_ptr<osg::Node>> SimpleModelNode::mNodes3D;
QMap<std::string, osg::ref_ptr<osg::Image>> SimpleModelNode::mImages2D;


SimpleModelNode::SimpleModelNode(MapItem *mapItem, const std::string &url3D, const std::string &url2D, QObject *parent)
    : QObject{parent},
    osgEarth::Annotation::ModelNode(mapItem->getMapNode(), Utility::getDefaultStyle()),
    mUrl3D(url3D),
    mMapItem(mapItem),
    mUrl2D(url2D)
{
    connect(mMapItem, &MapItem::modeChanged, this, &SimpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();

    mEnigine = QQmlEngine::contextForObject(mMapItem)->engine();
    compile();
    //--circle menu-------------------------------------------------------------
    createCircularMenu();

    //--node information window-------------------------------------------------
    createNodeInformation();

    createBookmarkItem();
}

SimpleModelNode::~SimpleModelNode()
{
    delete mNodeInformation;
    delete mCircularMenu;
}

MapItem *SimpleModelNode::mapItem() const
{
    return mMapItem;
}

void SimpleModelNode::updateUrl(const std::string &url3D, const std::string &url2D)
{
    if(mUrl2D != url2D || mUrl3D != url3D){
        mUrl2D = url2D;
        mUrl3D = url3D;
        compile();
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

BookmarkManager *SimpleModelNode::bookmarkManager() const
{
    return mBookmarkManager;
}

void SimpleModelNode::setBookmarkManager(BookmarkManager *bookmarkManager)
{
    mBookmarkManager = bookmarkManager;
    onBookmarkChecked(mIsBookmarked);
}

bool SimpleModelNode::isSelect() const
{
    return mIsSelected;
}

void SimpleModelNode::select(bool value)
{
    mIsSelected = value;
    //    setOutline(mIsSelected);
    mHighlightOutline->setEnabled(mIsSelected);
}

void SimpleModelNode::showMenu(bool value)
{
    mIsMenuVisible = value;
    mCircularMenu->show(mIsMenuVisible);
}

bool SimpleModelNode::isHighlight() const
{
    return mIsHighlight;
}

void SimpleModelNode::highlight(bool isHighlight)
{
    mIsHighlight = isHighlight;
    mSwitchMode->setValue(Highlight, isHighlight);
}

bool SimpleModelNode::isAutoScale() const
{
    return mIsAutoScale;
}

void SimpleModelNode::setAutoScale(bool newIsAutoScale)
{
    mIsAutoScale = newIsAutoScale;
    mAutoScaler->setScaled(newIsAutoScale);
    getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
//    if (mIsAutoScale){
//        setCullCallback(mAutoScaler);
//    }
//    else{
//        setCullCallback(nullptr);
//        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
//    }
}

NodeData *SimpleModelNode::nodeData() const
{
    return mNodeData;
}

void SimpleModelNode::setNodeData(NodeData *newNodeData)
{
    mNodeData = newNodeData;
    updateUrl(mNodeData->url3D, mNodeData->url2D);
    setColor(osgEarth::Color(mNodeData->color));
    if (mNodeInformation)
        mNodeInformation->setNodeData(newNodeData);
    if(mBookmarkItem)
        mBookmarkItem->setInfo(QString::fromStdString(mNodeData->type),
                               QString::fromStdString(mNodeData->name),
                               mNodeInformation->window(),
                               QString::fromStdString(mNodeData->imgSrc));

    //TODO add signal for update data--------------------
    setUserData(mNodeData);
}

osgEarth::Color SimpleModelNode::color() const
{
    return mColor;
}

void SimpleModelNode::setColor(osgEarth::Color color)
{
    //    if(mColor != color){
    //--recolor 3D Node----------------------------------------------------
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, color);
    m3DBaseNode->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
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
    //    }
    mColor = color;
}

bool SimpleModelNode::isAttacker() const
{
    return mIsAttacker;
}

void SimpleModelNode::setAttacker(bool attacker)
{
    mIsAttacker = attacker;

    if(attacker && !mCircularMenu->children().contains(mAttackerMenuItem)){
        mAttackerMenuItem = new CircularMenuItem{"Attack", "qrc:/Resources/menu-attack.png", false};
        QObject::connect(mAttackerMenuItem, &CircularMenuItem::itemClicked, this, &SimpleModelNode::onAttackChecked);

        mCircularMenu->appendMenuItem(mAttackerMenuItem);
    }
}

bool SimpleModelNode::is3D() const
{
    return mIs3D;
}

void SimpleModelNode::set2DHeaing(double heading)
{
    auto style = m2DNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::IconSymbol>()->heading() = heading;
    m2DNode->setStyle(style);
}

void SimpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    osg::ComputeBoundsVisitor cbv;

    if(mIs3D){
        mSwitchMode->setValue(Mode3D,true);
        mSwitchMode->setValue(Mode2D, false);
        setAutoScale(true);
    }
    else{
        mSwitchMode->setValue(Mode3D, false);
        mSwitchMode->setValue(Mode2D,true);
        setAutoScale(false);
    }

    //    mCircleSelectNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    //    mConeSelecteNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius()/4, osgEarth::Units::METERS));
    //    mConeSelecteNode->setHeight(osgEarth::Distance(cbv.getBoundingBox().radius()/2, osgEarth::Units::METERS));
    //    mConeSelecteNode->setCenter(osg::Vec3d(0,0,-mConeSelecteNode->getHeight().as(osgEarth::Units::METERS)/2));
    //    mConeSelecteNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,cbv.getBoundingBox().zMax()));
}

void SimpleModelNode::onInfoClicked()
{
    mNodeInformation->show();
}

void SimpleModelNode::onBookmarkChecked(bool status)
{
    if (!mBookmarkManager)
        return;
    mIsBookmarked = status;
    if (mIsBookmarked){
        mBookmarkManager->addBookmarkItem(mBookmarkItem);
    }
    else{
        mBookmarkManager->removeBookmarkItem(mBookmarkItem);
    }
}


void SimpleModelNode::onGoToPosition()
{
    mMapItem->getCameraController()->goToPosition(getPosition(), 500);
}

void SimpleModelNode::onTrack()
{
    mMapItem->getCameraController()->setTrackNode(getGeoTransform(), 400);
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
        m3DBaseNode = mNodes3D[mUrl3D];
    }
    else {
        m3DBaseNode = osgDB::readRefNodeFile(mUrl3D);
        mNodes3D[mUrl3D] = m3DBaseNode ;
    }
    //--auto scale-------------------------------------------------------
    double modelLenght = m3DBaseNode->getBound().radius() * 2;
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
//    if(mImage) mImage->setOrigin(osg::Image::TOP_LEFT);
    setCullingActive(false);
    mAutoScaler = new ModelAutoScaler(scaleRatio, 1, 1000);
    if (mIsAutoScale){
        setCullCallback(mAutoScaler);
    }
    //--root node--------------------------------------------------------
    mSwitchMode = new osg::Switch;
    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;
    m3DNode->addChild(m3DBaseNode, 0, std::numeric_limits<float>::max());

    mHighlightOutline = new HighlightOutline;
    mHighlightOutline->setWidth(6);
    mHighlightOutline->setColor(mSelectColor);
    mHighlightOutline->setEnabled(false);
    mHighlightOutline->addChild(m3DNode);

    //--2D node---------------------------------------------------------
//    m2DNode = new osg::Geode();
//    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
//    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
//    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImage, osg::Vec2s(0,0), 0, 0, 0.2);
//    m2DNode->setStateSet(geodeStateSet);
//    m2DNode->addDrawable(imgGeom);
    osgEarth::Symbology::Style st;
    st.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_CENTER;
    if(mImage) mImage->scaleImage(40, 42, mImage->r());
    m2DNode = new osgEarth::Annotation::PlaceNode("", st, mImage);
    m2DNode->setDynamic(true);
    m2DNode->setPriority(-1);
    //--set Highlite Node---------------------------------------------------
    //    osg::ref_ptr<osg::Group> selectGroup = new osg::Group;
    osg::ComputeBoundsVisitor cbv;
    if(mIs3D)
        m3DNode->accept(cbv);
    else{
        m2DNode->accept(cbv);
    }
    mConeHighliteNode = new Cone();
    mConeHighliteNode->setFillColor(osg::Vec4f(0.00392156862745098, 0.6823529411764706, 0.8392156862745098,0.15));
    mConeHighliteNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    mConeHighliteNode->setHeight(osgEarth::Distance(0, osgEarth::Units::METERS));
    //        mConeSelecteNode->setLocalRotation(osg::Quat(osg::PI,osg::Vec3d(1,1,0)));
    //        mConeSelecteNode->setCenter(osg::Vec3d(0,0,-mConeSelecteNode->getHeight().as(osgEarth::Units::METERS)/2));
    //        mConeSelecteNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,cbv.getBoundingBox().zMax()));
    //--highlight node-------------------------------------------------
    // mCircleHighlightNode = new Circle();
    // mCircleHighlightNode->setFillColor(osg::Vec4f(0,0.0,0.0,0));
    // mCircleHighlightNode->setStrokeColor(osg::Vec4f(0.12,1,1,0.5));
    // mCircleHighlightNode->setStrokeWidth(2);
    // mCircleHighlightNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius() - 0.1*cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    // mCircleHighlightNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.5));

    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchMode->insertChild(Mode3D, mHighlightOutline, true);
        mSwitchMode->insertChild(Mode2D, m2DNode->getPositionAttitudeTransform(), false);
    }
    else{
        mSwitchMode->insertChild(Mode3D, mHighlightOutline, false);
        mSwitchMode->insertChild(Mode2D, m2DNode->getPositionAttitudeTransform(), true);
        setAutoScale(false);
    }
    mSwitchMode->insertChild(Highlight, mConeHighliteNode, false);
    //--------------------------------------------------------------------------
    // this.
    osgEarth::Symbology::Style  rootStyle ;

    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mSwitchMode);
    //    rootStyle.getOrCreate<osgEarth::Symbology::Color(osgEarth::Color::Aqua)>();
    setStyle(rootStyle);

    setColor(mColor);

}

void SimpleModelNode::createCircularMenu()
{
    mCircularMenu = new CircularMenu(mMapItem, this);
    mCircularMenu->show(false);
    CircularMenuItem *infoMenuItem = new CircularMenuItem{"Info", "qrc:/Resources/menu-info.png", false, "qrc:/Resources/menu-info.png"};
    QObject::connect(infoMenuItem, &CircularMenuItem::itemClicked, this, &SimpleModelNode::onInfoClicked);

    mBookmarkMenuItem = new CircularMenuItem{"Bookmark", "qrc:/Resources/menu-bookmark.png", true, "qrc:/Resources/menu-bookmark-checked.png"};
    QObject::connect(mBookmarkMenuItem, &CircularMenuItem::itemChecked, this, &SimpleModelNode::onBookmarkChecked);

    CircularMenuItem *targetMenuItem = new CircularMenuItem{"Target", "qrc:/Resources/menu-target.png", false, "qrc:/Resources/menu-info.png"};
    QObject::connect(targetMenuItem, &CircularMenuItem::itemClicked, this, &SimpleModelNode::onTargetChecked);

    mCircularMenu->appendMenuItem(mBookmarkMenuItem);
    mCircularMenu->appendMenuItem(infoMenuItem);
    mCircularMenu->appendMenuItem(targetMenuItem);
}

void SimpleModelNode::createNodeInformation()
{
    mNodeInformation = new NodeInformation(mEnigine, mMapItem->window());

    connect(mNodeInformation,&NodeInformation::goToPosition, this, &SimpleModelNode::onGoToPosition);
    connect(mNodeInformation,&NodeInformation::track, this, &SimpleModelNode::onTrack);
}

void SimpleModelNode::createBookmarkItem()
{
    mBookmarkItem = new BookmarkItem();

    connect(mBookmarkItem, &BookmarkItem::goToPosition, this, &SimpleModelNode::onGoToPosition);

    connect(mBookmarkItem, &BookmarkItem::track, this, &SimpleModelNode::onTrack);

    connect(mBookmarkItem, &BookmarkItem::fromBookmarkRemoved, [&](){
        mIsBookmarked = false;
        mBookmarkMenuItem->checked = false;
    });
}

void SimpleModelNode::createSelectImage()
{
    int w = 5;
    for(int j=0; j<mImage->t(); ++j) {
        for(int i=0; i<mImage->s(); ++i) {
            //            std::cout<<"("<<QString::number(mImage->getColor(i, j).a(),'f',  2).toStdString()<<")";
            if(i > 0 && qAbs(mImage->getColor(i-1, j).a() - mSelectColor.a()) > 0.07
                &&qAbs(mImage->getColor(i, j).a() - mSelectColor.a()) > 0.07){
                if(mImage->getColor(i, j).a() < 0.1 && mImage->getColor(i - 1, j).a() > 0.1){
                    for(int a = 0; a < w-1; ++a)
                        if(i+a <= mImage->s())
                            mImage->setColor(mSelectColor, i+a, j);
                }
                if(mImage->getColor(i, j).a() > 0.1 && mImage->getColor(i-1, j).a() < 0.1){
                    for(int a = 1; a < w; ++a)
                        if(i-a >= 0)
                            mImage->setColor(mSelectColor, i-a, j);
                }

            }
        }
        //        std::cout<<std::endl;
    }
    mImage->dirty();
}


//void SimpleModelNode::setOutline(bool state)
//{
//    if (state){
//        mSwitchNode->removeChild(m3DNode);
//        mSwitchNode->insertChild(0, mHighlightOutline);
//        mHighlightOutline->setWidth(6);
//        mHighlightOutline->setColor(osg::Vec4(0.12,1,1,0.5));
//    } else {
//        mSwitchNode->removeChild(mHighlightOutline);
//        mSwitchNode->insertChild(0, m3DNode);
//        //        mHighlightLine->setColor(osg::Vec4(0.0,0.0,0.0,0));
//    }
//}
