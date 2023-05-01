#include "systemModelNode.h"
#include "polygon.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osg/Material>
#include "defenseModelNodeAutoScaler.h"
#include "truckf.h"
#include "trucks.h"
#include "truckl.h"
#include <QtMath>
#include <osgEarthAnnotation/CircleNode>
#include <osg/AutoTransform>

const float RANGE3D = 600;//std::numeric_limits<float>::max();;

SystemModelNode::SystemModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    mIs3D = mMapController->getMode();
    //--create root node--------------------------------------------------------------------------
    mRootNode = new osg::LOD;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);

    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler(3.5, 1, 600));


    setStyle(rootStyle);

    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

    static bool bFirst = true;
    static osg::ref_ptr<osg::Image> mainImage;
    static osg::ref_ptr<osg::Image> systemImageActive;
    static osg::ref_ptr<osg::Image> systemImageActiveHovered;
    static osg::ref_ptr<osg::Image> systemImageDeactive;
    static osg::ref_ptr<osg::Image> systemImageDeactiveHovered;

    if (bFirst) {
        mainImage = osgDB::readImageFile("../data/models/system/truck.png");

        systemImageActive = createColoredImage(mainImage, osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f));
        systemImageActiveHovered = createDarkerImage(systemImageActive, 0.5f);
        systemImageDeactive = createColoredImage(mainImage, osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f));
        systemImageDeactiveHovered = createDarkerImage(systemImageDeactive, 0.5f);


        if(systemImageActive)
            systemImageActive->scaleImage(100, 100, systemImageActive->r());
        if(systemImageActiveHovered)
            systemImageActiveHovered->scaleImage(100, 100, systemImageActiveHovered->r());
        if(systemImageDeactive)
            systemImageDeactive->scaleImage(100, 100, systemImageDeactive->r());
        if(systemImageDeactiveHovered)
            systemImageDeactiveHovered->scaleImage(100, 100, systemImageDeactiveHovered->r());


        bFirst = false;
    }



    osg::ref_ptr<osg::Geometry> systemImageDrawableActive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageActive, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  systemGeodeActive = new osg::Geode();
    systemGeodeActive->setStateSet(geodeStateSet);
    systemGeodeActive->addDrawable(systemImageDrawableActive);


    osg::ref_ptr<osg::Geometry> systemImageDrawableActiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageActiveHovered, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  systemGeodeActiveHovered = new osg::Geode();
    systemGeodeActiveHovered->setStateSet(geodeStateSet);
    systemGeodeActiveHovered->addDrawable(systemImageDrawableActiveHovered);

    osg::ref_ptr<osg::Geometry> systemImageDrawableDeactive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageDeactive, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  systemGeodeDeactive = new osg::Geode();
    systemGeodeDeactive->setStateSet(geodeStateSet);
    systemGeodeDeactive->addDrawable(systemImageDrawableDeactive);


    osg::ref_ptr<osg::Geometry> systemImageDrawableDeactiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageDeactiveHovered, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  systemGeodeDeactiveHovered = new osg::Geode();
    systemGeodeDeactiveHovered->setStateSet(geodeStateSet);
    systemGeodeDeactiveHovered->addDrawable(systemImageDrawableDeactiveHovered);



    osg::AutoTransform *at = new osg::AutoTransform;

    mNode2DActive = new osg::Switch;
    mNode2DActive->addChild(systemGeodeActive, true);
    mNode2DActive->addChild(systemGeodeActiveHovered, false);

    mNode2DDeactive = new osg::Switch;
    mNode2DDeactive->addChild(systemGeodeDeactive, true);
    mNode2DDeactive->addChild(systemGeodeDeactiveHovered, false);




    mNode2D = new osg::Switch;
    mNode2D->addChild(mNode2DActive, true);
    mNode2D->addChild(mNode2DDeactive, false);

    at->addChild(mNode2D);
    at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::ROTATE_TO_CAMERA);

    mTruckF = new TruckF(mMapController);
    mTruckF->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,5.0,0));
    mTruckS = new TruckS(mMapController);
    mTruckS->stopSearch();
    mTruckS->getPositionAttitudeTransform()->setPosition(osg::Vec3d(-5.0 * std::sin(qDegreesToRadians(60.0)), -5.0 * std::cos(qDegreesToRadians(60.0)),0));

    mTruckL = new TruckL(mMapController);
    mTruckL->getPositionAttitudeTransform()->setPosition(osg::Vec3d(5.0 * std::sin(qDegreesToRadians(60.0)), -5.0 * std::cos(qDegreesToRadians(60.0)),0));

    mNode3D = new Group;
    mNode3D->addChild(mTruckL);
    mNode3D->addChild(mTruckS);
    mNode3D->addChild(mTruckF);

    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;

    updateOrCreateLabelImage();
    mLabelNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);

    getGeoTransform()->addChild(mLabelNode);
    mLabelNode->setNodeMask(false);
    mLabelNode->setPriority(10);


    if(mIs3D)
    {
        mRootNode->addChild(mNode3D, 0, RANGE3D);
        mRootNode->addChild(at, RANGE3D, std::numeric_limits<float>::max());
    }
    else
    {
        mRootNode->addChild(mNode3D, 0, 0);
        mRootNode->addChild(at, 0, std::numeric_limits<float>::max());
    }

    mCircleNode = new osgEarth::Annotation::CircleNode();
    mCircleNode->setRadius(10);

    mCircleStyleActive.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f);
    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f);
    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

    mCircleNode->setStyle(mCircleStyleActive);
    mCircleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.05));
    mCircleNode->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);

    mNode3D->addChild(mCircleNode);


    //map mode changed-----------------------------------------------------------------------
    connect(mapControler, &MapController::modeChanged, this, &SystemModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new Circle(mMapController, true);
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.4f));

    mMezSphere = new SphereNode();
    mMezSphere->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.3f));
    mMezSphere->setSphereShape(SphereNode::SphereShape::SphereTopHalf);

    mWezPolygon = new Polygon(mMapController, false);
    mWezPolygon->setLineColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));
    mWezPolygon->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));

    if (!mSystemInformation) {
        mSystemInformation = new SystemInformation(mQmlEngine, mUIHandle, mInformation, mStatusInfo, mCombatInfo, this);
        connect(mSystemInformation->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
        connect(mSystemInformation->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
        connect(mSystemInformation->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
        connect(mSystemInformation->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
        connect(mSystemInformation->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
    }

}

void SystemModelNode::setInformation(const SystemInfo& info)
{
    if (mSystemInformation)
        mSystemInformation->setInfo(info);
    mInformation = info;
    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle());



}

SystemInfo SystemModelNode::getInformation() const
{
    return mInformation;
}

void SystemModelNode::setCombatInfo(const SystemCombatInfo &systemCombatInfo)
{
    mCombatInfo = systemCombatInfo;
    if (mSystemInformation)
        mSystemInformation->setCombatInfo(systemCombatInfo);

    switch (mCombatInfo.Phase) {
    case SystemCombatInfo::Search:
        searchPhase();
        break;
    case SystemCombatInfo::Lock:
        lockPhase(mCombatInfo.TN);
        break;
    case SystemCombatInfo::Fire:
        firePhase(mCombatInfo.TN);
        break;
    case SystemCombatInfo::Kill:
        killPhase(mCombatInfo.TN);
        break;
    case SystemCombatInfo::NoKill:
        noKillPhase(mCombatInfo.TN);
        break;
    }
    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle());
}

SystemCombatInfo SystemModelNode::getSystemCombatInfo() const
{
    return mCombatInfo;
}

void SystemModelNode::setStatusInfo(const SystemStatusInfo &systemStatusInfo)
{
    if (mSystemInformation)
        mSystemInformation->setStatusInfo(systemStatusInfo);
    mNode2D->setValue(0, systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S);
    mNode2D->setValue(1, systemStatusInfo.RadarSearchStatus != SystemStatusInfo::S);

    mCircleNode->setStyle(systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S ? mCircleStyleActive : mCircleStyleDeactive);
    mStatusInfo = systemStatusInfo;
    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle());
}

void SystemModelNode::addAssignment(int tn, AircraftModelNode *assignModelNode)
{
    if(!assignModelNode)
        return;
    if(!mAssignmentMap.contains(tn))
    {
        Assignment* assignmentModel = new  Assignment(mMapController);
        assignmentModel->mModelNode = assignModelNode;
        mAssignmentMap[tn] = assignmentModel;
        mSystemInformation->addAssignment(tn, assignModelNode);
        mMapController->addNodeToLayer(assignmentModel->mLine, SYSTEMS_LAYER_NAME);
    }
    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle());
}

AircraftModelNode *SystemModelNode::getAssignment(int tn) const
{
    if(!mAssignmentMap.contains(tn))
        return mAssignmentMap[tn]->mModelNode;
    return nullptr;
}

void SystemModelNode::acceptAssignment(int tn, bool value)
{
    if(mAssignmentMap.contains(tn))
    {
        if(value)
            mAssignmentMap[tn]->accept();
        else {
            removeAssignment(tn);
        }
    }
}

void SystemModelNode::removeAssignment(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        mMapController->removeNodeFromLayer(mAssignmentMap[tn]->mLine, SYSTEMS_LAYER_NAME);
        mAssignmentMap.remove(tn);
        mSystemInformation->removeAssignment(tn);
    }
    updateOrCreateLabelImage();
    mLabelNode->setStyle(mLabelNode->getStyle());
}

void SystemModelNode::clearAssignments(int exceptTN)
{
    for(auto tn: mAssignmentMap.keys())
    {
        if(tn != exceptTN)
        {
            removeAssignment(tn);
            mSystemInformation->removeAssignment(tn);
        }
    }
}

QMap<int, AircraftModelNode *> SystemModelNode::getAssignments() const
{
    QMap<int, AircraftModelNode *> assignModelNods;
    for(auto key: mAssignmentMap.keys())
        assignModelNods[key] = mAssignmentMap[key]->mModelNode;
    return assignModelNods;
}

void SystemModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
}

void SystemModelNode::onLeftButtonClicked(bool val)
{
    updateColors();
    //select(val);
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mMapController->untrackNode(getGeoTransform());
//        onRangeButtonToggled(val);
//        onWezButtonToggled(val);
//        onMezButtonToggled(val);
    }
}

void SystemModelNode::frameEvent()
{
    mLabelNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));

    for(auto assinmentModel:mAssignmentMap)
        assinmentModel->updateLine(getPosition());

    if (mTargetModelNode) {
        mTruckF->aimTarget(mTargetModelNode->getPosition());
        mTruckL->lockOnTarget(mTargetModelNode->getPosition());
    }
}

void SystemModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    DefenseModelNode::mousePressEvent(event, onModel);

    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
}

void SystemModelNode::updateColors()
{
    DefenseModelNode::updateColors();

    if (mSelectionMode == SELECTED || mHoverMode == HOVERED) {

        mNode2DActive->setValue(0, false);
        mNode2DActive->setValue(1, true);
        mNode2DDeactive->setValue(0, false);
        mNode2DDeactive->setValue(1, true);


    } else {

        mNode2DActive->setValue(0, true);
        mNode2DActive->setValue(1, false);
        mNode2DDeactive->setValue(0, true);
        mNode2DDeactive->setValue(1, false);

    }



}

void SystemModelNode::onModeChanged(bool is3DView)
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

    updateColors();
    //select(mIsSelected);
}

void SystemModelNode::onGotoButtonClicked()
{
    mMapController->goToPosition(getPosition(), 200);
}

void SystemModelNode::onRangeButtonToggled(bool check)
{
    if(check)
    {
        mRangeCircle->setPosition(getPosition());
        mRangeCircle->setRadius(osgEarth::Distance(mInformation.ViewRange, osgEarth::Units::METERS));

        mMapController->addNodeToLayer(mRangeCircle, SYSTEMS_LAYER_NAME);
    }
    else
    {
        mMapController->removeNodeFromLayer(mRangeCircle, SYSTEMS_LAYER_NAME);
    }
}

void SystemModelNode::onWezButtonToggled(bool checked)
{
    if(checked)
    {
        mWezPolygon->clearPoints();
        osg::Vec3d worldPosition;
        getPosition().toWorld(worldPosition, mMapController->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint;
        double radius = mInformation.MezRange;

        osg::Vec3d v1 = osg::Vec3d(worldPosition.x() - radius*2/4, worldPosition.y() - radius*2/4, worldPosition.z());
        osg::Vec3d v2 = osg::Vec3d(worldPosition.x() - radius*2/4, worldPosition.y() + radius*2/4, worldPosition.z());
        osg::Vec3d v3 = osg::Vec3d(worldPosition.x() + radius*2/4, worldPosition.y() + radius*2/4, worldPosition.z());
        osg::Vec3d v4 = osg::Vec3d(worldPosition.x() + radius*2/4, worldPosition.y() - radius*2/4, worldPosition.z());

        osgEarth::GeoPoint geoPoint1;
        geoPoint1.fromWorld(mMapController->getMapSRS(), v1);
        geoPoint1.z() = 0;
        geoPoint1.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mMapController->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint2;
        geoPoint2.fromWorld(mMapController->getMapSRS(), v2);
        geoPoint2.z() = 0;
        geoPoint2.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mMapController->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint3;
        geoPoint3.fromWorld(mMapController->getMapSRS(), v3);
        geoPoint3.z() = 0;
        geoPoint3.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mMapController->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint4;
        geoPoint4.fromWorld(mMapController->getMapSRS(), v4);
        geoPoint4.z() = 0;
        geoPoint4.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mMapController->getMapNode()->getTerrain());

        mWezPolygon->addPoints(geoPoint1);
        mWezPolygon->addPoints(geoPoint2);
        mWezPolygon->addPoints(geoPoint3);
        mWezPolygon->addPoints(geoPoint4);

        float height = static_cast<float>(radius/3);
        mWezPolygon->setHeight(height);

        mMapController->addNodeToLayer(mWezPolygon, SYSTEMS_LAYER_NAME);

    }
    else {
        mMapController->removeNodeFromLayer(mWezPolygon, SYSTEMS_LAYER_NAME);
    }
}

void SystemModelNode::onMezButtonToggled(bool checked)
{
    if(checked)
    {
        mMezSphere->setPosition(getPosition());
        mMezSphere->setRadius(mInformation.MezRange);
        mMapController->addNodeToLayer(mMezSphere, SYSTEMS_LAYER_NAME);
    }
    else
    {
        mMapController->removeNodeFromLayer(mMezSphere, SYSTEMS_LAYER_NAME);
    }
}

void SystemModelNode::onActiveButtonToggled(bool checked)
{
    mStatusInfo.RadarSearchStatus = (checked ? SystemStatusInfo::S : SystemStatusInfo::US);

    mNode2D->setValue(0, checked);
    mNode2D->setValue(1, !checked);

    mCircleNode->setStyle(checked ? mCircleStyleActive : mCircleStyleDeactive);
}

void SystemModelNode::searchPhase()
{
    for(auto assignmentModel: mAssignmentMap)
        assignmentModel->mLine->setColor(osgEarth::Color::Yellow);

    mTruckS->startSearch();
}

void SystemModelNode::lockPhase(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        mTargetModelNode = mAssignmentMap[tn]->mModelNode;
        mAssignmentMap[tn]->mLine->setColor(osgEarth::Color::Orange);

        mTruckL->lockOnTarget(mTargetModelNode->getPosition());
        mTruckF->aimTarget(mTargetModelNode->getPosition());

//        //remove other assigned models
//        for(auto assignModel:mAssignmentMap)
//        {
//            auto aircraftModelNode = static_cast<AircraftModelNode*>(assignModel->mModelNode);
//            if(aircraftModelNode && aircraftModelNode->getInformation().TN != tn)
//                removeAssignedModelNode(aircraftModelNode->getInformation().TN);
//        }
    }
}

void SystemModelNode::firePhase(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        mTargetModelNode = mAssignmentMap[tn]->mModelNode;
        mAssignmentMap[tn]->mLine->setColor(osgEarth::Color::Red);
        mFiredRocket = mTruckF->getActiveRocket();
        if(mFiredRocket)
        {
            mFiredRocket->setAutoScale();
            mTruckF->shoot(mAssignmentMap[tn]->mModelNode->getPosition().vec3d(), 2000);//1000 m/s
            mMapController->setTrackNode(mFiredRocket->getGeoTransform());
        }
    }
}

void SystemModelNode::killPhase(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        mMapController->untrackNode(mFiredRocket->getGeoTransform());
        mAssignmentMap[tn]->mLine->setColor(osgEarth::Color::Black);
        mAssignmentMap[tn]->mModelNode->collision();

        if(mFiredRocket)
            mFiredRocket->stop();

//        removeAssignment(tn);
    }
}

void SystemModelNode::noKillPhase(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        mMapController->untrackNode(mFiredRocket->getGeoTransform());
        mAssignmentMap[tn]->mLine->setColor(osgEarth::Color::Brown);
        if(mFiredRocket)
            mFiredRocket->stop();
//        removeAssignment(tn);
    }
}

void SystemModelNode::showInfoWidget()
{
//    if (!mSystemInformation) {
//        mSystemInformation = new SystemInformation(mQmlEngine, mUIHandle, mInformation, mStatusInfo, mCombatInfo, this);
//        connect(mSystemInformation->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
//        connect(mSystemInformation->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
//        connect(mSystemInformation->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
//        connect(mSystemInformation->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
//        connect(mSystemInformation->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
//    }
    mSystemInformation->setInfo(mInformation);
    mSystemInformation->show();
}

void SystemModelNode::updateOrCreateLabelImage()
{
//    qDebug() << ">>> : " << mAssignmentMap.keys().count();

    int height = LABEL_IMAGE_HEIGHT + ((mAssignmentMap.keys().count()+2)/3) * 30;
    if (mStatusInfo.Operational == SystemStatusInfo::Op2)
        height += 50;

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
        QPen  textPen(QColor(255, 255, 255));

        painter.setPen(Qt::NoPen);
        painter.setBrush(backgroundBrush);

        painter.drawRoundedRect(
                    mRenderTargetImage->rect(),
                    8,8);
        painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
        painter.drawRoundedRect(
                    QRect(0, 0, LABEL_IMAGE_WIDTH, 35),
                    8,8);
        //--------------------------------------------------------------
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
                         mInformation.Name);
        painter.drawText(QRect(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignCenter,
                         QString::number(mInformation.Number));
        //-------------------------------------------------------------
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Type:");
        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 40, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mInformation.Type);


        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "BCC:");
        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Radar:");

        if(mStatusInfo.BCCStatus == SystemStatusInfo::S)
            textPen.setColor(QColor(0, 255, 0));
        else
            textPen.setColor(QColor(255, 0, 0));
        painter.setPen(textPen);

        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 70, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mStatusInfo.radarStatusToString(mStatusInfo.BCCStatus));

        if(mStatusInfo.RadarSearchStatus == SystemStatusInfo::S)
            textPen.setColor(QColor(0, 255, 0));
        else
            textPen.setColor(QColor(255, 0, 0));
        painter.setPen(textPen);

        painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 100, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         mStatusInfo.radarStatusToString(mStatusInfo.RadarSearchStatus));




        //---------------------------------------------------------

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 135, LABEL_IMAGE_WIDTH, 135);

        textPen.setColor(QColor(255,255,255));
        painter.setPen(textPen);

        painter.drawText(QRect(10, 140, LABEL_IMAGE_WIDTH/2, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Assignments:");
        textPen.setColor(mCombatInfo.phaseToColor());
        painter.setPen(textPen);
        painter.drawText(QRect(10, 140, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString(mCombatInfo.phaseToString()));

        textPen.setColor(QColor(255,255,255));
        painter.setPen(textPen);

        int h = 170;
        const QFontMetrics fm(textFont);
        int n = 0;
        while (n < mAssignmentMap.values().count()) {

            int indent = 0;
            for (int llidx = 0; llidx < 3; llidx++)// two elements per line
            {

                if (n >= mAssignmentMap.values().count())
                    break;

                const auto val = mAssignmentMap.values()[n];

                const QString ss = (llidx == 0 ? QStringLiteral("") : QStringLiteral(", "))
                        + QString::number(val->mModelNode->getInformation().TN);

                textPen.setColor(QColor(255,255,255));
                painter.setPen(textPen);

                painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
                                 Qt::AlignLeft | Qt::AlignVCenter,
                                 ss);
                indent += fm.boundingRect(ss).width();

                n++;
            }

            h += 30;
        }


        //------------------------------------------------------------



        h = height - 60 - (mStatusInfo.Operational == SystemStatusInfo::Op2 ? 50 : 0);
        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, h, LABEL_IMAGE_WIDTH, h);

        h+=10;

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        static const QImage missleRedImage(":/resources/bullet_red.png");
        static const QImage missleGreenImage(":/resources/bullet_green.png");

        if (mStatusInfo.Operational == SystemStatusInfo::NoOp) {
            for (int i = 0; i < 9; i++) {
                painter.drawImage(
                            QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 9.0) * i, h, 20, 40),
                            missleRedImage,
                            missleRedImage.rect()
                            );

            }


        } else {
            for (int i = 0; i < 9; i++) {
                if(i < mStatusInfo.MissileCount) {
                    painter.drawImage(
                                QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 9.0) * i, h, 20, 40),
                                missleGreenImage,
                                missleGreenImage.rect()
                                );
                } else {
                    painter.drawImage(
                                QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 9.0) * i, h, 20, 40),
                                missleRedImage,
                                missleRedImage.rect()
                                );

                }
            }

            if (mStatusInfo.Operational == SystemStatusInfo::Op2) {
                h += 50;

                for (int i = 9; i < 18; i++) {
                    if(i < mStatusInfo.MissileCount) {
                        painter.drawImage(
                                    QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 9.0) * (i-9), h, 20, 40),
                                    missleGreenImage,
                                    missleGreenImage.rect()
                                    );
                    } else {
                        painter.drawImage(
                                    QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 9.0) * (i-9), h, 20, 40),
                                    missleRedImage,
                                    missleRedImage.rect()
                                    );

                    }
                }

            }
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



SystemModelNode::Assignment::Assignment(MapController *mapControler)
{
    mLine = new LineNode(mapControler);
    mLine->setPointVisible(true);
    mLine->setColor(osgEarth::Color::White);
    mLine->setPointColor(osgEarth::Color::Olive);
    mLine->setWidth(1);
    mLine->setPointWidth(5);
    mLine->setTessellation(15);
    mLine->setShowBearing(true);
}

void SystemModelNode::Assignment::accept()
{
    mLine->setColor(osgEarth::Color::Olive);
    mLine->setPointVisible(false);
    mLine->setWidth(5);
}

void SystemModelNode::Assignment::updateLine(const osgEarth::GeoPoint& position)
{
    mLine->clear();
    mLine->addPoint(position);
    mLine->addPoint(mModelNode->getPosition());
}
