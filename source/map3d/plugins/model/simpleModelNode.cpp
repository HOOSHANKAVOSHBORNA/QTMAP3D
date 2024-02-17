#include "simpleModelNode.h"
#include <osg/CullFace>
#include <osg/PolygonMode>
#include "mapItem.h"
#include <osgDB/WriteFile>
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

SimpleModelNode::SimpleModelNode(QQmlEngine *engine,
                                 MapItem *mapItem,
                                 const std::string &url3D,
                                 const std::string &url2D,
                                 QObject *parent)
    : QObject{parent}
    , mEngine(engine)
    , osgEarth::Annotation::ModelNode(mapItem->getMapNode(), Utility::getDefaultStyle())
    , mUrl3D(url3D)
    , mMapItem(mapItem)
    , mUrl2D(url2D)
{
    connect(mMapItem, &MapItem::modeChanged, this, &SimpleModelNode::onModeChanged);
    mIs3D = mMapItem->getMode();

    compile();
    //--circle menu-------------------------------------------------------------
    createCircularMenu();

    //--node information window-------------------------------------------------
    createNodeInformation();

    createBookmarkItem();
}

SimpleModelNode::~SimpleModelNode()
{
    if(mNodeInformation){
        delete mNodeInformation;
    }
    if(mCircularMenu){
        delete mCircularMenu;
    }
}

void SimpleModelNode::setQmlEngine(QQmlEngine *engine)
{
    mEngine = engine;
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
    if(!mIsHighlight){
        mOutlineNode->setColor(mSelectColor);
        mOutlineNode->setEnabled(mIsSelected);
    }
    setImageOutlinEnabled(value);
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

void SimpleModelNode::highlight(bool isHighlight, osgEarth::Color highliteColor)
{
    mHighliteColor = highliteColor;
    mIsHighlight = isHighlight;
    if(mIsHighlight){
        mOutlineNode->setColor(mHighliteColor);
        mOutlineNode->setEnabled(true);
    }
    else if(mIsSelected){
        mOutlineNode->setColor(mSelectColor);
    }
    else{
        mOutlineNode->setEnabled(false);
    }
    //    mSwitchMode->setValue(Highlight, isHighlight);
}

bool SimpleModelNode::isAutoScale() const
{
    return mIsAutoScale;
}

void SimpleModelNode::setAutoScale(bool newIsAutoScale)
{
    mIsAutoScale = newIsAutoScale;
    //    mAutoScaler->setScaled(newIsAutoScale);
    //    getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
    //    if (mIsAutoScale){
    //        setCullCallback(mAutoScaler);
    //    }
    //    else{
    //        setCullCallback(nullptr);
    //        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
    //    }
}

NodeData SimpleModelNode::nodeData() const
{
    return mNodeData;
}

void SimpleModelNode::setNodeData(const NodeData &nodeData)
{
    mNodeData = nodeData;
    //    updateUrl(mNodeData->url3D, mNodeData->url2D);
    //    setColor(osgEarth::Color(mNodeData->color));
    if (mNodeInformation)
        mNodeInformation->setNodeData(nodeData);
    if (mBookmarkItem)
        mBookmarkItem->setInfo(mNodeData.type.toString(),
                               mNodeData.name,
                               mNodeInformation->window(),
                               mNodeData.iconInfoUrl);

    //    //TODO add signal for update data--------------------
    setUserData(&mNodeData);
}

osgEarth::Color SimpleModelNode::color() const
{
    return mColor;
}

void SimpleModelNode::setColor(osgEarth::Color color)
{
    if(mColor != color){
        //--recolor 3D Node----------------------------------------------------
        osg::ref_ptr<osg::Material> mat = new osg::Material;
        mat->setDiffuse(osg::Material::FRONT_AND_BACK, color);
        m3DNode->getOrCreateStateSet()->setAttributeAndModes(mat,
                                                             osg::StateAttribute::ON
                                                                 | osg::StateAttribute::OVERRIDE);
        //--recolor 2D Node----------------------------------------------------
        osg::Vec4 imageColor = color;
        for (int i = 0; i < mImage->s(); ++i) {
            for (int j = 0; j < mImage->t(); ++j) {
                imageColor.a() = mImage->getColor(i, j).a();
                mImage->setColor(imageColor, i, j);
            }
        }
        mImage->dirty();
        //---------------------------------------------------------------------
    }
    mColor = color;
}

bool SimpleModelNode::isAttacker() const
{
    return mIsAttacker;
}

void SimpleModelNode::setAttacker(bool attacker)
{
    mIsAttacker = attacker;

    //    if(attacker && !mCircularMenu->children().contains(mAttackerMenuItem)){
    //        mAttackerMenuItem = new CircularMenuItem{"Attack", "qrc:/Resources/menu-attack.png", false};
    //        QObject::connect(mAttackerMenuItem, &CircularMenuItem::itemClicked, this, &SimpleModelNode::onAttackChecked);

    //        mCircularMenu->appendMenuItem(mAttackerMenuItem);
    //    }
    if(mIsAttacker)
        mCircularMenu->appendMenuItem(mAttackerMenuItem);
    else
        mCircularMenu->removeMenuItem(mAttackerMenuItem);
}

bool SimpleModelNode::is3D() const
{
    return mIs3D;
}

void SimpleModelNode::set2DHeaing(double heading)
{
    //    auto style = m2DNode->getStyle();
    //    style.getOrCreate<osgEarth::Symbology::IconSymbol>()->heading() = heading;
    //    m2DNode->setStyle(style);
}

void SimpleModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    osg::ComputeBoundsVisitor cbv;

    if(mIs3D){
        mSwitchMode->setValue(Mode3D,true);
        mSwitchMode->setValue(Mode2D, false);
        //        setAutoScale(true);
    }
    else{
        mSwitchMode->setValue(Mode3D, false);
        mSwitchMode->setValue(Mode2D,true);
        //        setAutoScale(false);
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
    osg::ref_ptr<osg::Image> image;
    if (mImages2D.contains(mUrl2D)){
        image = mImages2D[mUrl2D];
    }
    else {
        image = osgDB::readImageFile(mUrl2D);
        mImages2D[mUrl2D] = image ;
    }
    //--generate low model path ------------------------------
    QUrl lowUrl3D(QString::fromStdString(mUrl3D));
    lowUrl3D = lowUrl3D.resolved("low" + lowUrl3D.fileName());
    //--------------------------------------------------------
    mImage = new osg::Image(*image, osg::CopyOp::DEEP_COPY_ALL);
    if (mNodes3D.contains(mUrl3D)){
        m3DBaseNode = mNodes3D[mUrl3D];
        m3DLowNode = mNodes3D["../" +lowUrl3D.toString().toStdString()];

    }
    else {
        m3DBaseNode = osgDB::readRefNodeFile(mUrl3D);
        mNodes3D[mUrl3D] = m3DBaseNode ;
        if((m3DLowNode = osgDB::readRefNodeFile("../" +lowUrl3D.toString().toStdString()))){
            mNodes3D["../" +lowUrl3D.toString().toStdString()] = m3DLowNode;
        }
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
    //    qDebug()<<"iconSize: "<<iconSize;
    if(mImage){
        mImage->scaleImage(iconSize, iconSize, mImage->r());
        createOutlineImage();
    }
    setCullingActive(false);
    mAutoScaler = new AutoScaler(scaleRatio, 1, 1000);
    setCullCallback(mAutoScaler);
    //--root node--------------------------------------------------------
    mSwitchMode = new osg::Switch;
    //--3D node----------------------------------------------------------
    m3DNode = new osg::LOD;
    if(m3DLowNode){
        m3DNode->addChild(m3DBaseNode, 0 , 100);
        m3DNode->addChild(m3DLowNode , 100 , std::numeric_limits<float>::max() );
    }else{
        m3DNode->addChild(m3DBaseNode , 0 , std::numeric_limits<float>::max() );
    }
    mOutlineNode = new HighlightOutline;
    mOutlineNode->setWidth(6);
    mOutlineNode->setColor(mSelectColor);
    mOutlineNode->setEnabled(false);
    mOutlineNode->addChild(m3DNode);

    //    osgEarth::Symbology::Style  modelStyle ;
    //    modelStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mHighlightOutline);
    //    osg::ref_ptr<osgEarth::Annotation::ModelNode> transform3D = new osgEarth::Annotation::ModelNode(mMapItem->getMapNode(), Utility::getDefaultStyle());
    //    transform3D->setCullingActive(false);
    //    transform3D->setCullCallback(mAutoScaler);
    //    transform3D->setStyle(modelStyle);

    //--2D node---------------------------------------------------------
    m2DNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    osgEarth::ScreenSpaceLayoutOptions option;
    option.technique() = osgEarth::ScreenSpaceLayoutOptions::TECHNIQUE_LABELS;
    option.leaderLineMaxLength() = 64;
    option.leaderLineWidth() = 32;
    osgEarth::ScreenSpaceLayout::setOptions(option);
    //        osgEarth::ScreenSpaceLayout::activate(geodeStateSet.get());
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    //        geodeStateSet->setRenderBinDetails(
    //            13,
    //            OSGEARTH_SCREEN_SPACE_LAYOUT_BIN,
    //            osg::StateSet::OVERRIDE_PROTECTED_RENDERBIN_DETAILS);
    //        geodeStateSet->setNestRenderBins( false );
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImage, osg::Vec2s(0,0), 0, 0, 0.2);
    m2DNode->setStateSet(geodeStateSet);
    m2DNode->addDrawable(imgGeom);

    //    m3DNode->addChild(_imageDrawable, 0, std::numeric_limits<float>::max());
    //    osgEarth::Symbology::Style st;
    //    st.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_CENTER;
    //    st.getOrCreate<osgEarth::Symbology::TextSymbol>()->onScreenRotation() = 90;
    ////    st.getOrCreate<osgEarth::Symbology::BBoxSymbol>()->fill()->color() = osgEarth::Color::Green;
    //    m2DNode = new osgEarth::Annotation::PlaceNode("", st, mImage);
    //    m2DNode->setDynamic(true);
    //    m2DNode->setPriority(-1);
    //    m2DNode->setOcclusionCulling(true);
    //--set Highlite Node---------------------------------------------------
    //    osg::ref_ptr<osg::Group> selectGroup = new osg::Group;
    osg::ComputeBoundsVisitor cbv;
    if(mIs3D)
        m3DNode->accept(cbv);
    else{
        m2DNode->accept(cbv);
    }
    //    mConeHighliteNode = new Cone();
    //    mConeHighliteNode->setFillColor(osg::Vec4f(0.00392156862745098, 0.6823529411764706, 0.8392156862745098,0.15));
    //    mConeHighliteNode->setRadius(osgEarth::Distance(cbv.getBoundingBox().radius(), osgEarth::Units::METERS));
    //    mConeHighliteNode->setHeight(osgEarth::Distance(0, osgEarth::Units::METERS));
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
    //    m2DNode->addChild(mConeHighliteNode);
    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchMode->insertChild(Mode3D, mOutlineNode, true);
        mSwitchMode->insertChild(Mode2D, m2DNode, false);
    }
    else{
        mSwitchMode->insertChild(Mode3D, mOutlineNode, false);
        mSwitchMode->insertChild(Mode2D, m2DNode, true);
        //        setAutoScale(false);
    }
    //    mSwitchMode->insertChild(Highlight, mConeHighliteNode, false);
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

    mAttackerMenuItem = new CircularMenuItem{"Attack", "qrc:/Resources/menu-attack.png", false};
    QObject::connect(mAttackerMenuItem, &CircularMenuItem::itemClicked, this, &SimpleModelNode::onAttackChecked);

    mCircularMenu->appendMenuItem(mBookmarkMenuItem);
    mCircularMenu->appendMenuItem(infoMenuItem);
    mCircularMenu->appendMenuItem(targetMenuItem);
}

void SimpleModelNode::createNodeInformation()
{
    mNodeInformation = new NodeInformation(mEngine, mMapItem->window());

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

void SimpleModelNode::setImageOutlinEnabled(bool value)
{
    for(int j = 0; j < mImage->t(); ++j) {
        for(int i = 0; i < mImage->s(); ++i) {
            if(mOutlineImage->getColor(i, j).a() > 0.1){
                if(value)
                    mImage->setColor(mSelectColor, i, j);
                else
                    mImage->setColor(osg::Vec4(0.0,0.0,0.0,0.0), i, j);
            }

        }
    }
    mImage->dirty();
}

void SimpleModelNode::createOutlineImage()
{
    mOutlineImage = new osg::Image(*mImage, osg::CopyOp::DEEP_COPY_ALL);
    int w = 3;
    for(int j = w; j < mImage->t() - w; ++j) {
        for(int i = w; i < mImage->s() - w; ++i) {
            if(mImage->getColor(i, j).a() > 0.1){
                if( mImage->getColor(i-1, j).a() < 0.1)
                    for(int a = 0; a < w; a++)
                        mOutlineImage->setColor(mSelectColor, i-a, j);
                if(mImage->getColor(i + 1, j).a() < 0.1)
                    for(int a = 0; a < w; a++)
                        mOutlineImage->setColor(mSelectColor, i+a, j);
                if(mImage->getColor(i, j - 1).a() < 0.1)
                    for(int a = 0; a < w; a++)
                        mOutlineImage->setColor(mSelectColor, i, j-a);
                if(mImage->getColor(i, j + 1).a() < 0.1)
                    for(int a = 0; a < w; a++)
                        mOutlineImage->setColor(mSelectColor, i, j + a);

                mOutlineImage->setColor(osg::Vec4(0.0,0.0,0.0,0.0), i, j);
            }
        }
    }
    //    bool resultSnap = osgDB::writeImageFile(*mOutlineImage, "/home/client110/Pictures/mOutlineImage.png");
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
