#include "stationmodelnode.h"
#include "systemmodelnode.h"
#include "truck.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Material>

const int RANGE3D = 500;

StationModelNode::StationModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :BaseModel(mapControler->getMapNode(), parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    //--create root node---------------------------------------------------------------------------
    mRootNode = new osg::LOD;
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
    //--create 2d node----------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/station/station_ll_red.png");
    redIcon->scaleImage(32, 32, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    redGeode->addDrawable(redImageDrawable);
//    auto redPlaceNode = new osgEarth::Annotation::PlaceNode();
//    redPlaceNode->setIconImage(redIcon);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/station/station_ll_yell.png");
    yellowIcon->scaleImage(32, 32, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);
//    auto yellowPlaceNode = new osgEarth::Annotation::PlaceNode();
//    yellowPlaceNode->setIconImage(yellowIcon);

    mNode2D = new osg::Switch;
    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);
//    mNode2D->addChild(yellowPlaceNode, false);
//    mNode2D->addChild(redPlaceNode, true);
    //--create 3D node---------------------------------------------------------------------------
    osg::ref_ptr<Truck> truck = new Truck(getMapNode());
    mNode3D = truck;
//    truck->setQStringName("truck");
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    mLableNode = new osgEarth::Annotation::PlaceNode(getName(),labelStyle, lableImage);
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0.5, 1));
    getGeoTransform()->addChild(mLableNode);
    mLableNode->setNodeMask(false);
    //--add nods--------------------------------------------------------------------------------
    if(mIs3D)
    {
        mRootNode->addChild(mNode3D, 0, RANGE3D);
        mRootNode->addChild(mNode2D, RANGE3D, std::numeric_limits<float>::max());
    }
    else
    {
        mRootNode->addChild(mNode3D, 0, 0);
        mRootNode->addChild(mNode2D, 0, std::numeric_limits<float>::max());
    }
    //map mode changed-----------------------------------------------------------------------
    connect(mapControler, &MapController::modeChanged, this, &StationModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new Circle(mMapController, true);
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));

    mVisiblePolygone = new Polygone(mMapController, true);
    mVisiblePolygone->setLineColor(osg::Vec4(0.0, 1.0, 0.0, 0.5f));
    mVisiblePolygone->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.5f));
}

void StationModelNode::setInformation(const StationInfo& info)
{
    mInformation = info;
}
void StationModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
    //mMapController->goToPosition(getPosition(), 200);
}

void StationModelNode::onLeftButtonClicked(bool val)
{
    select(val);
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mMapController->untrackNode();
    }
}

void StationModelNode::frameEvent()
{
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
}

void StationModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    BaseModel::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
}

void StationModelNode::onGotoButtonClicked()
{
    mMapController->goToPosition(getPosition(), 200);
}

void StationModelNode::onRangeButtonToggled(bool check)
{
    if(check)
    {
        mRangeCircle->setPosition(getPosition());
        mRangeCircle->setRadius(osgEarth::Distance(mInformation.Radius, osgEarth::Units::METERS));
        mMapController->addNode(mRangeCircle);
    }
    else
    {
        mMapController->removeNode(mRangeCircle);
    }
}

void StationModelNode::onVisibleButtonToggled(bool checked)
{
    if(checked)
    {
        mVisiblePolygone->addPoints(osg::Vec3d(getPosition().x() - 0.089,getPosition().y() + 0.059, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() + 0.001,getPosition().y() + 0.089, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() + 0.101,getPosition().y() + 0.059, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() + 0.151,getPosition().y() + 0.009, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() + 0.061,getPosition().y() - 0.091, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() - 0.059,getPosition().y() - 0.121, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() - 0.099,getPosition().y() - 0.061, 0));
        mVisiblePolygone->addPoints( osg::Vec3d(getPosition().x() - 0.119,getPosition().y() + 0.029, 0));

        mMapController->addNode(mVisiblePolygone);
    }
    else
    {
        mMapController->removeNode(mVisiblePolygone);
        mVisiblePolygone->clearPoints();
    }
}

void StationModelNode::onActivateButtonToggled(bool checked)
{

}

void StationModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D)
    {
        mRootNode->setRange(0, 0, RANGE3D);
        mRootNode->setRange(1, RANGE3D, std::numeric_limits<float>::max());
    }
    else
    {
        mRootNode->setRange(0, 0, 0);
        mRootNode->setRange(1,0, std::numeric_limits<float>::max());
    }

    select(mIsSelected);
}

void StationModelNode::showInfoWidget()
{
    StationInformtion *stationInformation = new StationInformtion(mQmlEngine, mUIHandle, mInformation, this);
    connect(stationInformation->getInfo(), &StationInfoModel::gotoButtonClicked, this, &StationModelNode::onGotoButtonClicked);
    connect(stationInformation->getInfo(), &StationInfoModel::rangeButtonClicked, this, &StationModelNode::onRangeButtonToggled);
    connect(stationInformation->getInfo(), &StationInfoModel::visibleButtonClicked, this, &StationModelNode::onVisibleButtonToggled);
    connect(stationInformation->getInfo(), &StationInfoModel::activateButtonClicked, this, &StationModelNode::onActivateButtonToggled);
    stationInformation->show();
}
