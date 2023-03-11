#include "systemModelNode.h"
#include "polygone.h"

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

const float RANGE3D = std::numeric_limits<float>::max();;

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

    osg::ref_ptr<osg::Image> redIcon = createColoredImage(osgDB::readImageFile("../data/models/system/images/truck.png"), osg::Vec4(1.0, 0.0, 0.0, 1.0));
    if(redIcon)
        redIcon->scaleImage(100, 100, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    redGeode->setStateSet(geodeStateSet);
    redGeode->addDrawable(redImageDrawable);

    osg::ref_ptr<osg::Image> yellowIcon = createColoredImage(osgDB::readImageFile("../data/models/system/images/truck.png"), osg::Vec4(1.0, 1.0, 0.0, 1.0));
    if(yellowIcon)
        yellowIcon->scaleImage(100, 100, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 0.3);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->setStateSet(geodeStateSet);
    yellowGeode->addDrawable(yellowImageDrawable);

    osg::AutoTransform *at = new osg::AutoTransform;


    mNode2D = new osg::Switch;
    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);

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
    mLableNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);

    getGeoTransform()->addChild(mLableNode);
    mLableNode->setNodeMask(false);


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

    auto circleNode = new osgEarth::Annotation::CircleNode();
    circleNode->setRadius(10);

    osgEarth::Symbology::Style style;
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.1f, 1.0f, 0.1f, 1.0f);
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

    circleNode->setStyle(style);
    circleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.05));
    circleNode->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);

    mNode3D->addChild(circleNode);


    //map mode changed-----------------------------------------------------------------------
    connect(mapControler, &MapController::modeChanged, this, &SystemModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new CircleNode(mMapController, true);
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.4f));

    mMezSphere = new SphereNode();
    mMezSphere->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.3f));
    mMezSphere->setSphereShape(SphereNode::SphereShape::SphereTopHalf);

    mWezPolygon = new Polygone(mMapController, false);
    mWezPolygon->setLineColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));
    mWezPolygon->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));

}

void SystemModelNode::setInformation(const SystemInfo& info)
{
    if (mSystemInformation)
        mSystemInformation->setInfo(info);
    mInformation = info;
    updateOrCreateLabelImage();
}

SystemInfo SystemModelNode::getInformation() const
{
    return mInformation;
}

void SystemModelNode::setCambatInfo(const SystemCambatInfo &systemCambatInfo)
{
    mCambatInfo = systemCambatInfo;
    if (mSystemInformation)
        mSystemInformation->setCombatInfo(systemCambatInfo);

    switch (mCambatInfo.Phase) {
    case SystemCambatInfo::Search:
        searchPhase();
        break;
    case SystemCambatInfo::Lock:
        lockPhase(mCambatInfo.TN);
        break;
    case SystemCambatInfo::Fire:
        firePhase(mCambatInfo.TN);
        break;
    case SystemCambatInfo::Kill:
        killPhase(mCambatInfo.TN);
        break;
    case SystemCambatInfo::NoKill:
        noKillPhase(mCambatInfo.TN);
        break;
    }
}

void SystemModelNode::setStatusInfo(const SystemStatusInfo &systemStatusInfo)
{
    if (mSystemInformation)
        mSystemInformation->setStatusInfo(systemStatusInfo);
    mStatusInfo = systemStatusInfo;
    updateOrCreateLabelImage();
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
        addNodeToLayer(assignmentModel->mLine);
    }
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
        else
            removeAssignment(tn);
    }
}

void SystemModelNode::removeAssignment(int tn)
{
    if(mAssignmentMap.contains(tn))
    {
        removeNodeFromLayer(mAssignmentMap[tn]->mLine);
        mAssignmentMap.remove(tn);
    }
}

void SystemModelNode::clearAssignments(int exceptTN)
{
    for(auto tn: mAssignmentMap.keys())
    {
        if(tn != exceptTN)
        {
            removeAssignment(tn);
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
    select(val);
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mMapController->untrackNode(getGeoTransform());
        onRangeButtonToggled(val);
        onWezButtonToggled(val);
        onMezButtonToggled(val);
    }
}

void SystemModelNode::frameEvent()
{
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));

    for(auto assinmentModel:mAssignmentMap)
        assinmentModel->updateLine(getPosition());

    if (mTargetModelNode) {
        mTruckF->aimTarget(mTargetModelNode->getPosition());
        mTruckL->lockOnTarget(mTargetModelNode->getPosition());
    }
}

void SystemModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
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

    select(mIsSelected);
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

        addNodeToLayer(mRangeCircle);
    }
    else
    {
        removeNodeFromLayer(mRangeCircle);
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

        mWezPolygon->addPoints(geoPoint1.vec3d());
        mWezPolygon->addPoints(geoPoint2.vec3d());
        mWezPolygon->addPoints(geoPoint3.vec3d());
        mWezPolygon->addPoints(geoPoint4.vec3d());

        float height = static_cast<float>(radius/3);
        mWezPolygon->setHeight(height);

        addNodeToLayer(mWezPolygon, true);

    }
    else {
        removeNodeFromLayer(mWezPolygon);
    }
}

void SystemModelNode::onMezButtonToggled(bool checked)
{
    if(checked)
    {
        mMezSphere->setPosition(getPosition());
        mMezSphere->setRadius(mInformation.MezRange);
        addNodeToLayer(mMezSphere);
    }
    else
    {
        removeNodeFromLayer(mMezSphere);
    }
}

void SystemModelNode::onActiveButtonToggled(bool checked)
{
    mInformation.Active = checked;
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
            mTruckF->shoot(mAssignmentMap[tn]->mModelNode->getPosition().vec3d(), 20000);//1000 m/s
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

bool SystemModelNode::addNodeToLayer(osg::Node *node, bool insert)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(SYSTEMS_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            if(insert)
                group->insertChild(0,node);
            else
                group->addChild(node);
        }
    }
}

bool SystemModelNode::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(SYSTEMS_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
void SystemModelNode::showInfoWidget()
{
    mSystemInformation = new SystemInformation(mQmlEngine, mUIHandle, mInformation, mStatusInfo, mCambatInfo, this);
    connect(mSystemInformation->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
    connect(mSystemInformation->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
    connect(mSystemInformation->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
    connect(mSystemInformation->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
    connect(mSystemInformation->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
    mSystemInformation->show();
}

void SystemModelNode::updateOrCreateLabelImage()
{
    if (!mRenderTargetImage) {
        mRenderTargetImage = new QImage(
                    LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
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

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH, 30),
                         Qt::AlignCenter,
                         mInformation.Name);


        static const QPen linePen(QColor(255, 255, 255),
                                  1,
                                  Qt::PenStyle::DashLine
                                  );

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 35, LABEL_IMAGE_WIDTH, 35);


        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Number:");
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(mInformation.Number));


        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "BCC:");
        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Radar:");

        if(mStatusInfo.BCCStatus == SystemStatusInfo::S)
            textPen.setColor(QColor(0, 255, 0));
        else
            textPen.setColor(QColor(255, 0, 0));
        painter.setPen(textPen);

        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mStatusInfo.radarStatusToString(mStatusInfo.BCCStatus));

        if(mStatusInfo.RadarSearchStatus == SystemStatusInfo::S)
            textPen.setColor(QColor(0, 255, 0));
        else
            textPen.setColor(QColor(255, 0, 0));
        painter.setPen(textPen);

        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mStatusInfo.radarStatusToString(mStatusInfo.RadarSearchStatus));



        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 135, LABEL_IMAGE_WIDTH, 135);


        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        static const QImage missleRedImage(":/resources/bullet_red.png");
        static const QImage missleGreenImage(":/resources/bullet_green.png");
        for (int i = 0; i < 6; i++) {
            if(i < mStatusInfo.MissileCount) {
                painter.drawImage(
                            QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 6.0) * i, 143, 20, 40),
                            missleGreenImage,
                            missleGreenImage.rect()
                            );
            } else {
                painter.drawImage(
                            QRect(10 + ((LABEL_IMAGE_WIDTH - 20.0) / 6.0) * i, 143, 20, 40),
                            missleRedImage,
                            missleRedImage.rect()
                            );

            }
        }

    }
    *mRenderTargetImage = mRenderTargetImage->mirrored(false, true);

    mLabelImage->setImage(LABEL_IMAGE_WIDTH,
                          LABEL_IMAGE_HEIGHT,
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
