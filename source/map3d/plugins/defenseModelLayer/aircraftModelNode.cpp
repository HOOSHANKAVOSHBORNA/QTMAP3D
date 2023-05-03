#include "aircraftModelNode.h"
#include "contextMenu.h"
#include "aircraftInfoItem.h"
#include "draw.h"
#include "systemModelNode.h"
#include "defenseModelLayer.h"

#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <osg/Depth>
#include <osg/Material>
#include<osg/Switch>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTimer>
#include <array>
#include "defenseModelNodeAutoScaler.h"


#include "defenseModelLayer.h"

#include <QFontMetrics>

const float RANGE3D = std::numeric_limits<float>::max();//832
const int NUM_LATEST_POINT = 100;

osg::ref_ptr<osg::Node> AircraftModelNode::mAircraft3DRef;
osg::ref_ptr<osg::Node> AircraftModelNode::mFighter3DRef;
osg::ref_ptr<osg::Node> AircraftModelNode::mMissile3DRef;
osg::ref_ptr<osg::Node> AircraftModelNode::mDrone3DRef;
osg::ref_ptr<osg::Node> AircraftModelNode::mHelicopter3DRef;

AircraftModelNode::AircraftModelNode(DefenseModelLayer *defenseModelLayer, QList<int> *assignments, AircraftInfo::AircraftType aircraftType, QObject *parent)
    :DefenseModelNode(defenseModelLayer->mMapController, parent)
{

    //mQmlEngine = qmlEngine;
    mAssignments = assignments;
    mDefenseModelLayer = defenseModelLayer;
    mIs3D = mDefenseModelLayer->mMapController->getMode();

    //mUIHandle = uiHandle;
    //--load models----------------------------------------------------------------------------------
    if (!mAircraft3DRef.valid()) {
        mAircraft3DRef = osgDB::readRefNodeFile("../data/models/aircraft/boeing-747.osgb");
    }
    if (!mFighter3DRef.valid()) {
        mFighter3DRef = osgDB::readRefNodeFile("../data/models/fighter/fighter.osgb");
    }
    if (!mMissile3DRef.valid()) {
        mMissile3DRef = osgDB::readRefNodeFile("../data/models/missile/missile.osgb");
    }
    if (!mDrone3DRef.valid()) {
        mDrone3DRef = osgDB::readRefNodeFile("../data/models/drone/drone.osgb");
    }
    if (!mHelicopter3DRef.valid()) {
        mHelicopter3DRef = osgDB::readRefNodeFile("../data/models/hellicopter/HellicopterUC.osgb");
    }
    //----------------------------------------------------------------------------------------------
//    if (!mNode3DRef)
//    {
//        return;
//    }
    this->setCullingActive(false);
    mNode3D = new osg::Group;
    switch (aircraftType) {
    case AircraftInfo::Fighter:
        mAutoScaleDefaultValue = 11;
        mAutoScaleMinValue = 7;
        mAutoScaleMaxValue = 350;
        mNode3D->addChild(mFighter3DRef);
        break;
    case AircraftInfo::Aircraft:
        mAutoScaleDefaultValue = 2.5;
        mAutoScaleMinValue = 1;
        mAutoScaleMaxValue = 500;
        mNode3D->addChild(mAircraft3DRef);
        break;
    case AircraftInfo::Missile:
        mAutoScaleDefaultValue = 25;
        mAutoScaleMinValue = 15;
        mAutoScaleMaxValue = 500;
        mNode3D->addChild(mMissile3DRef);
        break;
    case AircraftInfo::Drone:
        mAutoScaleDefaultValue = 30;
        mAutoScaleMinValue = 20;
        mAutoScaleMaxValue = 400;
        mNode3D->addChild(mDrone3DRef);
        break;
    case AircraftInfo::Helicopter:
        mAutoScaleDefaultValue = 22;
        mAutoScaleMinValue = 12;
        mAutoScaleMaxValue = 500;
        mNode3D->addChild(mHelicopter3DRef);
        break;
    }
    this->addCullCallback(mDefenseModeNodeAutoScaler);

    mRootNode = new osg::LOD;
    mNode2D = new osg::Switch;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);





    setStyle(rootStyle);

    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

    mNode2DNormal = new osg::Switch;
    mNode2DHovered = new osg::Switch;

    static bool bFirst = true;
    static osg::ref_ptr<osg::Image> mainImageAircraft;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListAircraft;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHoveredAircraft;

    static osg::ref_ptr<osg::Image> mainImageDrone;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListDrone;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHoveredDrone;

    static osg::ref_ptr<osg::Image> mainImageFighter;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListFighter;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHoveredFighter;

    static osg::ref_ptr<osg::Image> mainImageMissile;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListMissile;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHoveredMissile;

    static osg::ref_ptr<osg::Image> mainImageHellicopter;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHellicopter;
    static std::array<osg::ref_ptr<osg::Image>, 6> imageListHoveredHellicopter;


    static const osgEarth::Color colorList[6]= {
        osg::Vec4(0.2f, 0.8f, 0.2f, 1.0f),
        osg::Vec4(0.8f, 0.8f, 0.2f, 1.0f),
        osg::Vec4(0.8f, 0.5f, 0.2f, 1.0f),
        osg::Vec4(0.8f, 0.2f, 0.2f, 1.0f),
        osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f),
        osg::Vec4(0.8f, 0.2f, 0.2f, 1.0f),
    };


    if (bFirst) {

        mainImageAircraft = osgDB::readImageFile("../data/models/aircraft/aircraft.png");
        mainImageMissile = osgDB::readImageFile("../data/models/missile/missle.png");
        mainImageDrone = osgDB::readImageFile("../data/models/drone/drone.png");
        mainImageFighter = osgDB::readImageFile("../data/models/fighter/fighter.png");
        mainImageHellicopter = osgDB::readImageFile("../data/models/hellicopter/hellicopter.png");


        for (unsigned int i = 0; i < 6; i++) {
            osg::ref_ptr<osg::Image> aircraftImage = createColoredImage(mainImageAircraft, colorList[i]);
            if(aircraftImage)
                aircraftImage->scaleImage(100, 100, aircraftImage->r());


            osg::ref_ptr<osg::Image> aircraftImageHovered = createDarkerImage(aircraftImage, 0.5f);
            if(aircraftImageHovered)
                aircraftImageHovered->scaleImage(100, 100, aircraftImageHovered->r());

            imageListAircraft[i] = aircraftImage;
            imageListHoveredAircraft[i] = aircraftImageHovered;
        }

        for (unsigned int i = 0; i < 6; i++) {
            osg::ref_ptr<osg::Image> aircraftImage = createColoredImage(mainImageDrone, colorList[i]);
            if(aircraftImage)
                aircraftImage->scaleImage(100, 100, aircraftImage->r());


            osg::ref_ptr<osg::Image> aircraftImageHovered = createDarkerImage(aircraftImage, 0.5f);
            if(aircraftImageHovered)
                aircraftImageHovered->scaleImage(100, 100, aircraftImageHovered->r());

            imageListDrone[i] = aircraftImage;
            imageListHoveredDrone[i] = aircraftImageHovered;
        }

        for (unsigned int i = 0; i < 6; i++) {
            osg::ref_ptr<osg::Image> aircraftImage = createColoredImage(mainImageFighter, colorList[i]);
            if(aircraftImage)
                aircraftImage->scaleImage(100, 100, aircraftImage->r());


            osg::ref_ptr<osg::Image> aircraftImageHovered = createDarkerImage(aircraftImage, 0.5f);
            if(aircraftImageHovered)
                aircraftImageHovered->scaleImage(100, 100, aircraftImageHovered->r());

            imageListFighter[i] = aircraftImage;
            imageListHoveredFighter[i] = aircraftImageHovered;
        }

        for (unsigned int i = 0; i < 6; i++) {
            osg::ref_ptr<osg::Image> aircraftImage = createColoredImage(mainImageMissile, colorList[i]);
            if(aircraftImage)
                aircraftImage->scaleImage(100, 100, aircraftImage->r());


            osg::ref_ptr<osg::Image> aircraftImageHovered = createDarkerImage(aircraftImage, 0.5f);
            if(aircraftImageHovered)
                aircraftImageHovered->scaleImage(100, 100, aircraftImageHovered->r());

            imageListMissile[i] = aircraftImage;
            imageListHoveredMissile[i] = aircraftImageHovered;
        }

        for (unsigned int i = 0; i < 6; i++) {
            osg::ref_ptr<osg::Image> aircraftImage = createColoredImage(mainImageHellicopter, colorList[i]);
            if(aircraftImage)
                aircraftImage->scaleImage(100, 100, aircraftImage->r());


            osg::ref_ptr<osg::Image> aircraftImageHovered = createDarkerImage(aircraftImage, 0.5f);
            if(aircraftImageHovered)
                aircraftImageHovered->scaleImage(100, 100, aircraftImageHovered->r());

            imageListHellicopter[i] = aircraftImage;
            imageListHoveredHellicopter[i] = aircraftImageHovered;
        }


        bFirst = false;
    }

    for (unsigned int i = 0; i < 6; i++) {

        osg::ref_ptr<osg::Image> img;
        osg::ref_ptr<osg::Image> imgHovered;

        switch (aircraftType) {
        case AircraftInfo::Aircraft:
            img = imageListAircraft[i];
            imgHovered = imageListHoveredAircraft[i];
            break;
        case AircraftInfo::Fighter:
            img = imageListFighter[i];
            imgHovered = imageListHoveredFighter[i];
            break;
        case AircraftInfo::Drone:
            img = imageListDrone[i];
            imgHovered = imageListHoveredDrone[i];
            break;
        case AircraftInfo::Missile:
            img = imageListMissile[i];
            imgHovered = imageListHoveredMissile[i];
            break;
        case AircraftInfo::Helicopter:
            img = imageListHellicopter[i];
            imgHovered = imageListHoveredHellicopter[i];
            break;
        }

        osg::ref_ptr<osg::Geometry> aircraftImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry
                (img, osg::Vec2s(0,0), 0, 0, 0.4);
        osg::ref_ptr<osg::Geode>  aircraftGeode = new osg::Geode();
        aircraftGeode->setStateSet(geodeStateSet);
        aircraftGeode->addDrawable(aircraftImageDrawable);


        osg::ref_ptr<osg::Geometry> aircraftImageDrawableHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry
                (imgHovered, osg::Vec2s(0,0), 0, 0, 0.4);
        osg::ref_ptr<osg::Geode>  aircraftGeodeHovered = new osg::Geode();
        aircraftGeodeHovered->setStateSet(geodeStateSet);
        aircraftGeodeHovered->addDrawable(aircraftImageDrawableHovered);

        mNode2DNormal->addChild(aircraftGeode, false);
        mNode2DHovered->addChild(aircraftGeodeHovered, false);

    }

    mNode2DNormal->setValue(0, true);
    mNode2DHovered->setValue(0, true);


//    if(m2DIcon)
//        m2DIcon->scaleImage(100, 100, m2DIcon->r());
//    mSelect2DIcon = new osg::Image;
//    mSelect2DIcon->copySubImage(0, 0, 0, m2DIcon);
//


//    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(m2DIcon, osg::Vec2s(0,0), 0, 0, 0.4);
//    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
//
//
//    redGeode->setStateSet(geodeStateSet);
//    redGeode->addDrawable(redImageDrawable);
//
//    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mSelect2DIcon, osg::Vec2s(0,0), 0, 0, 0.4);
//    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
//    yellowGeode->setStateSet(geodeStateSet);
//    yellowGeode->addDrawable(yellowImageDrawable);


    mNode2D->addChild(mNode2DNormal, true);
    mNode2D->addChild(mNode2DHovered, false);

    mPat2D = new osg::PositionAttitudeTransform;
    mPat2D->setAttitude(osg::Quat(osg::inDegrees(45.0), -osg::Z_AXIS));

    mPat2D->addChild(mNode2D);

    osg::AutoTransform *at = new osg::AutoTransform;

    at->addChild(mPat2D);
    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);

    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;

    updateOrCreateLabelImage();
    mLabelNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);
    mLabelNode->setDynamic(true);
    mLabelNode->setPriority(10);


    getGeoTransform()->addChild(mLabelNode);
    mLabelNode->setNodeMask(false);

    if(mIs3D)
    {
        mRootNode->addChild(mNode3D, 0, RANGE3D);
        mRootNode->addChild(at, RANGE3D, std::numeric_limits<float>::max());

        mDefenseModeNodeAutoScaler->setDefaultScale(mAutoScaleDefaultValue);
        mDefenseModeNodeAutoScaler->setMinScale(mAutoScaleMinValue);
        mDefenseModeNodeAutoScaler->setMaxScale(mAutoScaleMaxValue);

    }
    else
    {
        mRootNode->addChild(mNode3D, 0, 0);
        mRootNode->addChild(at, 0, std::numeric_limits<float>::max());

        mDefenseModeNodeAutoScaler->setDefaultScale(2.5);
        mDefenseModeNodeAutoScaler->setMinScale(1);
        mDefenseModeNodeAutoScaler->setMaxScale(500);
    }


    float radius = getBound().radius();
    float scale = 3;

    mFire = new osgParticle::FireEffect(osg::Vec3f(0, 2*radius,0),scale,100.0);
    mFire->setUseLocalParticleSystem(false);

    mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, 2*radius,0),scale/3,100.0);
    mSmoke->setUseLocalParticleSystem(false);

    mLocationPoints = new osg::Vec3Array();
    mTempLocationPoints = new osg::Vec3Array();


    connect(mDefenseModelLayer->mMapController, &MapController::modeChanged, this, &AircraftModelNode::onModeChanged);

    mRouteLine = new LineNode(defenseModelLayer->mMapController);
    mRouteLine->setPointVisible(false);
    mRouteLine->setTessellation(10);
    mRouteLine->setColor(osgEarth::Color::Purple);
    mRouteLine->setWidth(5);
    mRouteLine->showLenght(false);

    mLatestPointLine = new LineNode(defenseModelLayer->mMapController);
    mLatestPointLine->setPointVisible(true);
    mLatestPointLine->setPointColor(osgEarth::Color::Blue);
    mLatestPointLine->setColor(osgEarth::Color::Purple);
    mLatestPointLine->setWidth(5);
    mLatestPointLine->setPointWidth(15);
    mLatestPointLine->showLenght(false);
    mLatestPointLine->setSmooth(true);

    mTempLine = new LineNode(defenseModelLayer->mMapController);
    mTempLine->setPointVisible(false);
    mTempLine->setColor(osgEarth::Color::Purple);
    mTempLine->setWidth(5);
    mTempLine->showLenght(false);
}

void AircraftModelNode::flyTo(osgEarth::GeoPoint posGeo, double heading, double /*speed*/)
{

    posGeo.transformInPlace(mDefenseModelLayer->mMapController->getMapSRS());
    osg::Vec3d currentPosW;
    getPosition().toWorld(currentPosW);

    osg::Vec3d posW;
    posGeo.toWorld(posW);

    osg::Vec3d diffW = posW - currentPosW;
    osg::Matrixd currentPoslocalTransfer;
    getPosition().createWorldToLocal(currentPoslocalTransfer);
    osg::Vec3d diffLocal = diffW * osg::Matrixd::rotate(currentPoslocalTransfer.getRotate());
    diffLocal.normalize();

    osg::Quat diffRotate;
    diffRotate.makeRotate(osg::Vec3d(0, 1, 0), diffLocal);

    osg::Vec3d northVec(0, 1, 0);//in local
    osg::Vec3d headingVecLocal = northVec * osg::Matrixd::rotate(osg::inDegrees(-heading), osg::Z_AXIS);
    headingVecLocal.normalize();

    headingVecLocal.z() = diffLocal.z();

    osg::Matrixd posLocalToWorld;
    posGeo.createLocalToWorld(posLocalToWorld);


    osg::Quat headingRotate;
    headingRotate.makeRotate(osg::Vec3d(0, 1, 0), headingVecLocal);


    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentPosW,getPositionAttitudeTransform()->getAttitude(),getScale()));
    path->insert(0.001,osg::AnimationPath::ControlPoint(posW,headingRotate, getScale()));




    mAnimationPathCallback = new ModelAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);


    if(mRouteLine->getSize() <= 0)
    {
        mRouteLine->addPoint(getPosition());
        mLatestPointLine->addPoint(getPosition());
        mCurrentFlyPoint = getPosition();
    }
    mLatestPointLine->addPoint(mCurrentFlyPoint);
    if(mLatestPointLine->getSize() >= NUM_LATEST_POINT)
    {
        mLatestPointLine->removeFirstPoint();
    }
    if(std::abs(mCurrentHeading - heading) < 5)
        mRouteLine->removePoint();
    mRouteLine->addPoint(mCurrentFlyPoint);

    mCurrentHeading = heading;
    mCurrentFlyPoint = posGeo;
    mTempLine->clear();
}

void AircraftModelNode::stop()
{
    mIsStop = true;
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->getAnimationPath()->clear();
    removeEffect();
}

void AircraftModelNode::setInformation(AircraftInfo info)
{
    mInformation = info;

    if(mAircraftinformation)
        mAircraftinformation->updateAircraft(info);

    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle()); // force PlaceNode to recreate texture

    changeModelColor(mInformation.Identification);
}

AircraftInfo AircraftModelNode::getInformation() const
{
    return mInformation;
}

void AircraftModelNode::goOnTrack()
{
    mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onLeftButtonClicked(bool val)
{
    //select(val);
    updateColors();
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mDefenseModelLayer->mMapController->untrackNode(getGeoTransform());
//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
        if(mAircraftinformation)
            mAircraftinformation->setTrackOff();
    }
    if(mCurrentContextMenu){
        mCurrentContextMenu->hideMenu();
        mCurrentContextMenu = nullptr;
    }
}

void AircraftModelNode::frameEvent()
{
    mPat2D->setAttitude(osg::Quat(osg::inDegrees(-double(mDefenseModelLayer->mMapController->getViewpoint().getHeading())
                                                 + mInformation.Heading),
                                  -osg::Z_AXIS));

    mPat2D->setPosition(osg::Vec3d(0, 0, 0));

    if (mCurrentContextMenu) {
        osg::Vec3d wordPos;
        getPosition().toWorld(wordPos);
        float x, y;
        mDefenseModelLayer->mMapController->worldToScreen(wordPos,x, y);
        mCurrentContextMenu->updatePosition(static_cast<int>(x), static_cast<int>(y));
    }


    mLabelNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));

    if(mTempLine->getSize() > 1)
        mTempLine->removePoint();
    mTempLine->addPoint(getPosition());
}

void AircraftModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    DefenseModelNode::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
    if(event->button() == Qt::RightButton) {
        mCurrentContextMenu = new ContextMenu(mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle, this);
        for(auto detectSystem: mInformation.DetectionSystems)
            mCurrentContextMenu->addRow(detectSystem);

        connect(mCurrentContextMenu->getModel(), &ContextMenumodel::itemClicked, this, &AircraftModelNode::onContextmenuItemClicked);
        osg::Vec3d wordPos;
        getPosition().toWorld(wordPos);
        float x, y;
        mDefenseModelLayer->mMapController->worldToScreen(wordPos,x, y);
        mCurrentContextMenu->show(static_cast<int>(x), static_cast<int>(y));
        event->accept();
    }
    if(!onModel && mCurrentContextMenu){
        mCurrentContextMenu->hideMenu();
        mCurrentContextMenu = nullptr;
    }

}

void AircraftModelNode::updateColors()
{
    DefenseModelNode::updateColors();

    if (mSelectionMode == SELECTED || mHoverMode == HOVERED) {
        mNode2D->setValue(0, false);
        mNode2D->setValue(1, true);
    } else {
        mNode2D->setValue(0, true);
        mNode2D->setValue(1, false);
    }
}

//SystemModelNode *AircraftModelNode::getAssignment(int number) const
//{
//    if(mAssignmentMap.contains(number))
//        return mAssignmentMap[number];
//    return nullptr;
//}

//void AircraftModelNode::addAssignment(int number, SystemModelNode *assignmentModelNode)
//{
//    mAssignmentMap[number] = assignmentModelNode;
//    mAircraftinformation->addAssignment(number, assignmentModelNode);
//}

//void AircraftModelNode::removeAssignment(int number)
//{
//    mAssignmentMap.remove(number);
//    mAircraftinformation->removeAssignment(number);
//}

//void AircraftModelNode::acceptAssignment(int number, bool value)
//{
//    if(!value)
//        removeAssignment(number);
//}

//void AircraftModelNode::clearAssignments(int exceptNumber)
//{
//    for(auto number: mAssignmentMap.keys())
//    {
//        if(exceptNumber != number)
//        {
//            removeAssignment(number);
//            mAircraftinformation->removeAssignment(number);
//        }
//    }
//}

//bool AircraftModelNode::hasAssignment()
//{
//    return mAssignmentMap.count() > 0;
//}

//QMap<int, SystemModelNode *> AircraftModelNode::getAssignments() const
//{
//    return mAssignmentMap;
//}

void AircraftModelNode::onGotoButtonClicked()
{
    //    goOnTrack();
    mDefenseModelLayer->mMapController->goToPosition(getPosition(), 400, 0);
    mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onRouteButtonToggled(bool check)
{
    if(check)
    {
        mDefenseModelLayer->mMapController->addNodeToLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
        mDefenseModelLayer->mMapController->addNodeToLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
    }
    else
    {
        mDefenseModelLayer->mMapController->removeNodeFromLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
        mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
    }

}

void AircraftModelNode::onLatestPointsToggled(bool check) {
    if (check)
    {
        mDefenseModelLayer->mMapController->addNodeToLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
        mDefenseModelLayer->mMapController->addNodeToLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
    }
    else
    {
        mDefenseModelLayer->mMapController->removeNodeFromLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
        mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
    }
}

void AircraftModelNode::onTrackButtonToggled(bool check)
{
    //std::cout << check << std::endl;
    if(check)
        mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
    else
        mDefenseModelLayer->mMapController->untrackNode(getGeoTransform());
}

void AircraftModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D)
    {
        mRootNode->setRange(0, 0, RANGE3D);
        mRootNode->setRange(1, RANGE3D, std::numeric_limits<float>::max());

        mDefenseModeNodeAutoScaler->setDefaultScale(mAutoScaleDefaultValue);
        mDefenseModeNodeAutoScaler->setMinScale(mAutoScaleMinValue);
        mDefenseModeNodeAutoScaler->setMaxScale(mAutoScaleMaxValue);

    }
    else
    {
        mRootNode->setRange(0, 0, 0);
        mRootNode->setRange(1,0, std::numeric_limits<float>::max());


        mDefenseModeNodeAutoScaler->setDefaultScale(2.5);
        mDefenseModeNodeAutoScaler->setMinScale(1);
        mDefenseModeNodeAutoScaler->setMaxScale(500);
    }

}

void AircraftModelNode::onContextmenuItemClicked(int index,  QString systemName)
{
    //std::cout << index << ", " << systemName.toStdString() << std::endl;
}

void AircraftModelNode::changeModelColor(AircraftInfo::Identify identify)
{

    for (unsigned int i = 0; i < 6; i++) {
        mNode2DNormal->setValue(i, false);
        mNode2DHovered->setValue(i, false);
    }

    osgEarth::Color color;
    switch (identify) {
    case AircraftInfo::F:
        color = osgEarth::Color::Green;
        mNode2DNormal->setValue(0, true);
        mNode2DHovered->setValue(0, true);
        break;
    case AircraftInfo::K:
        color = osgEarth::Color::Yellow;
        mNode2DNormal->setValue(1, true);
        mNode2DHovered->setValue(1, true);
        break;
    case AircraftInfo::Z:
        color =  osg::Vec4(1.0, 0.5, 0.0, 1.0);
        mNode2DNormal->setValue(2, true);
        mNode2DHovered->setValue(2, true);
        break;
    case AircraftInfo::X:
        color = osgEarth::Color::Red;
        mNode2DNormal->setValue(3, true);
        mNode2DHovered->setValue(3, true);
        break;
    case AircraftInfo::U:
        color = osgEarth::Color::White;
        mNode2DNormal->setValue(4, true);
        mNode2DHovered->setValue(4, true);
        break;
    case AircraftInfo::H:
        color = osgEarth::Color::Red;
        mNode2DNormal->setValue(5, true);
        mNode2DHovered->setValue(5, true);
        break;

//    default:
//        color = osgEarth::Color::Green;
//        mNode2DNormal->setValue(0, true);
//        mNode2DHovered->setValue(0, true);
//        break;
    }
    mModelColor = color;

    //change2DImageColore(mModelColor);

    updateColors();
    //select(mIsSelected);
}

void AircraftModelNode::showInfoWidget()
{
    if (!mAircraftinformation)
    {
        mAircraftinformation = new AircraftInfoItem(mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle, mInformation, this);
        connect(mAircraftinformation->getInfo(), &AircraftInfoModel::gotoButtonClicked, this, &AircraftModelNode::onGotoButtonClicked);
        connect(mAircraftinformation->getInfo(), &AircraftInfoModel::routeButtonClicked, this, &AircraftModelNode::onRouteButtonToggled);
        connect(mAircraftinformation->getInfo(), &AircraftInfoModel::trackButtonClicked, this, &AircraftModelNode::onTrackButtonToggled);
        connect(mAircraftinformation->getInfo(), &AircraftInfoModel::latestPointsClicked, this, &AircraftModelNode::onLatestPointsToggled);
    }
    mAircraftinformation->show();
}

void AircraftModelNode::addEffect(double emitterDuration)
{

    osgEarth::Registry::shaderGenerator().run(mFire);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mFire);
    mFire->setEmitterDuration(emitterDuration);
    mFire->setParticleDuration(0.2);
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());// for textures or lighting
    mDefenseModelLayer->mMapController->addNodeToLayer(mFire->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
    //add smoke----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mSmoke);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mSmoke);
    mSmoke->setEmitterDuration(emitterDuration);
    mSmoke->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());// for textures or lighting
    mDefenseModelLayer->mMapController->addNodeToLayer(mSmoke->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
}

void AircraftModelNode::removeEffect()
{
    //remove fire---------------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mFire->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mSmoke->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
    getPositionAttitudeTransform()->removeChild(mSmoke);
}

void AircraftModelNode::change2DImageColore(osgEarth::Color /*color*/)
{
//    if(!m2DIcon)
//        return;
//
//    unsigned width = static_cast<unsigned>(m2DIcon->s());
//    unsigned height = static_cast<unsigned>(m2DIcon->t());
//    for(unsigned i = 0; i < width; i++)
//        for(unsigned j = 0; j < height; j++)
//        {
//            osg::Vec4 pixColore = m2DIcon->getColor(i, j);
//
//            if(pixColore.a()>0)
//                m2DIcon->setColor(color, i, j);
//        }
//
//    color /= 2;
//    color.a() = 1;
//
//    width = static_cast<unsigned>(m2DIcon->s());
//    height = static_cast<unsigned>(m2DIcon->t());
//
//    for(unsigned i = 0; i < width; i++) {
//        for(unsigned j = 0; j < height; j++)
//        {
//            osg::Vec4 pixColore = mSelect2DIcon->getColor(i, j);
//
//            if(pixColore.r()>0 || pixColore.g()>0 || pixColore.b()>0)
//                mSelect2DIcon->setColor(color, i, j);
//        }
//    }
}


void AircraftModelNode::updateOrCreateLabelImage()
{
    int height = LABEL_IMAGE_HEIGHT + ((mAssignments->count()+1)/2) * 30;
    //qDebug()<<"hight:"<<height;
    if (!mRenderTargetImage) {
        mRenderTargetImage = new QImage(
                    LABEL_IMAGE_WIDTH,
                    height,
                    QImage::Format_RGBA8888
                    );
    } else {
        mRenderTargetImage->~QImage();
        mRenderTargetImage = new(mRenderTargetImage) QImage(
                    LABEL_IMAGE_WIDTH,
                    height,
                    QImage::Format_RGBA8888
                    );
    }

    if (!mLabelImage) {
        mLabelImage = new osg::Image;
    }


    {
        mRenderTargetImage->fill(QColor(Qt::transparent));
        QPainter painter(mRenderTargetImage);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);


        static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));

        static const QFont textFont("SourceSansPro", 12, QFont::Normal);
        static QPen  textPen(QColor(255, 255, 255));

        painter.setPen(Qt::NoPen);
        painter.setBrush(backgroundBrush);

        painter.drawRoundedRect(
                    mRenderTargetImage->rect(),
                    8,8);
        painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
        painter.drawRoundedRect(
                    QRect(0, 0, LABEL_IMAGE_WIDTH, 35),
                    8,8);
        //------------------------------------------------------------
        static const QPen linePen(QColor(255, 255, 255),
                                  1,
                                  Qt::PenStyle::DashLine
                                  );

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 35, LABEL_IMAGE_WIDTH, 35);
        painter.drawLine(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, 35);

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignCenter,
                         mInformation.aircraftTypeToString());
        painter.drawText(QRect(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignCenter,
                         QString::number(mInformation.TN));
        //-------------------------------------------------------------

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "CallSign:");
        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 40, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mInformation.CallSign);


        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "IFFCode:");
        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 70, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mInformation.IFFCode);


        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "M-Radar:");
        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 100, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mInformation.MasterRadar);

        painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "I-Method:");
        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 130, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mInformation.IdentificationMethod);
        //---------------------------------------------------------

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 165, LABEL_IMAGE_WIDTH, 165);

        painter.setPen(textPen);

        painter.drawText(QRect(10, 170, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Assignments:");

        int h = 200;
        const QFontMetrics fm(textFont);
        int n = 0;
        while (n < mAssignments->count()) {

            int indent = 0;
            for (int llidx = 0; llidx < 2; llidx++)// two elements per line
            {

                if (n >= mAssignments->count())
                    break;

                int val = mAssignments->at(n);

                const QString ss = (llidx == 0 ? QStringLiteral("(") : QStringLiteral(", ("))
                        + QString::number(val)
                        + QStringLiteral(", ");
                const QString cc /*= QString(val->getSystemCombatInfo().phaseToString()[0])*/;
                const QString ee = QStringLiteral(")");

                textPen.setColor(QColor(255,255,255));
                painter.setPen(textPen);

                painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
                                 Qt::AlignLeft | Qt::AlignVCenter,
                                 ss);
                indent += fm.boundingRect(ss).width();

//                textPen.setColor(val->getSystemCombatInfo().phaseToColor());
                painter.setPen(textPen);

                painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
                                 Qt::AlignLeft | Qt::AlignVCenter,
                                 cc);
                indent += fm.boundingRect(cc).width();

                textPen.setColor(QColor(255,255,255));
                painter.setPen(textPen);

                painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
                                 Qt::AlignLeft | Qt::AlignVCenter,
                                 ee);
                indent += fm.boundingRect(ee).width();


                n++;
            }

            h += 30;
        }


    }
    *mRenderTargetImage = mRenderTargetImage->mirrored(false, true);

    mLabelImage->setImage(LABEL_IMAGE_WIDTH,
                          height,
                          1,
                          GL_RGBA,
                          GL_RGBA,
                          GL_UNSIGNED_BYTE,
                          mRenderTargetImage->bits(),
                          osg::Image::AllocationMode::NO_DELETE);
}


