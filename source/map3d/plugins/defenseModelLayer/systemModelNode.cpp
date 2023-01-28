#include "systemModelNode.h"
#include "polygone.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Material>

const float RANGE3D = std::numeric_limits<float>::max();;

SystemModelNode::SystemModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    mIs3D = mMapController->getMode();
    //--create root node--------------------------------------------------------------------------
    mRootNode = new osg::LOD;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    setStyle(rootStyle);
    //--create 2D Nodes---------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/system/system_red.png");
    redIcon->scaleImage(32, 32, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    redGeode->addDrawable(redImageDrawable);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/system/system_yell.png");
    yellowIcon->scaleImage(32, 32, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);

    mNode2D = new osg::Switch;
    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);
    //--create 3D node---------------------------------------------------------------------------
    mTruck = new Truck(mMapController, this);
    mNode3D = mTruck;
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    //osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    updateOrCreateLabelImage();
    mLableNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);
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
    connect(mapControler, &MapController::modeChanged, this, &SystemModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new Circle(mMapController, true);
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
    mInformation = info;
    setDisplayText(mInformation.Name);
}

void SystemModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
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

void SystemModelNode::setMissleCount(int numMissles)
{
    mMissleCount = numMissles;
    updateOrCreateLabelImage();
}

void SystemModelNode::setDisplayText(QString displayText)
{
    if(mDisplayText != displayText)
    {
        mDisplayText = displayText;
        updateOrCreateLabelImage();
    }
}

void SystemModelNode::setBCCStatus(QString bccStatus)
{
    mBCCStatus = bccStatus;
    updateOrCreateLabelImage();
}

void SystemModelNode::setRadarSearchStatus(QString radarSearchStatus)
{
    mRadarSearchStatus = radarSearchStatus;
    updateOrCreateLabelImage();
}

int SystemModelNode::getMissleCount() const
{
    return mMissleCount;
}

QString SystemModelNode::getDisplayText() const
{
    return mDisplayText;
}

QString SystemModelNode::getBCCStatus() const
{
    return mBCCStatus;
}

QString SystemModelNode::getRadarSearchStatus() const
{
    return mRadarSearchStatus;
}

void SystemModelNode::collision()
{
    if(mAssignedModelNode && mFiredRocket)
    {
        osg::Vec3d wAssignedPosition;
        mAssignedModelNode->getPosition().toWorld(wAssignedPosition);
        osg::Vec3d wRocketPosition;
        mFiredRocket->getPosition().toWorld(wRocketPosition);
        double distance = (wAssignedPosition - wRocketPosition).length();
        if(distance < 3 && !mHit)
        {
            mAssignedModelNode->collision();
            mFiredRocket->collision();
            mMapController->removeNode(mAssignedLine->getNode());
            mHit = true;
        }
    }
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
        mMapController->untrackNode();
        onRangeButtonToggled(val);
        onWezButtonToggled(val);
        onMezButtonToggled(val);
    }
}

void SystemModelNode::showInfoWidget()
{
    SystemInformation *systemInformation = new SystemInformation(mQmlEngine, mUIHandle, mInformation, this);
    connect(systemInformation->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
    connect(systemInformation->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
    connect(systemInformation->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
    connect(systemInformation->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
    connect(systemInformation->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
    systemInformation->show();
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


        static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, 255));

        static const QFont textFont("SourceSansPro", 12, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));

        painter.setPen(Qt::NoPen);
        painter.setBrush(backgroundBrush);

        painter.drawRoundedRect(
                    mRenderTargetImage->rect(),
                    8,8);

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH, 30),
                         Qt::AlignCenter,
                         mDisplayText);


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
                         "MissleNo");
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(mMissleCount));


        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "BCC");
        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mBCCStatus);


        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Radar");
        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mRadarSearchStatus);



        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 135, LABEL_IMAGE_WIDTH, 135);


        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        static const QImage missleRedImage(":/resources/bullet_red.png");
        static const QImage missleGreenImage(":/resources/bullet_green.png");
        for (int i = 0; i < 6; i++) {
            if(i < mMissleCount) {
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

DefenseModelNode *SystemModelNode::getAssignedModelNode() const
{
    return mAssignedModelNode;
}

void SystemModelNode::setAssignedModelNode(DefenseModelNode *assignedModelNode)
{
    if(!assignedModelNode)
        return;
    unassignedModelNode();

    mAssignedModelNode = assignedModelNode;
    mAssignedLine = new Line(mMapController);
    mAssignedLine->setLineClamp(false);
    mAssignedLine->setLineColor(osgEarth::Color::Green);
    mAssignedLine->setLineWidth(6);
    mMapController->addNode(mAssignedLine->getNode());

    mTruck->aimTarget(mAssignedModelNode->getPosition().vec3d());
}

void SystemModelNode::unassignedModelNode()
{
    if(mAssignedModelNode)
    {
        mMapController->removeNode(mAssignedLine->getNode());
    }
}

void SystemModelNode::fire()
{
    if(mAssignedModelNode)
    {
        mFiredRocket = mTruck->getActiveRocket();
        if(mFiredRocket)
        {
            mAssignedModelNode->stop();//TODO for test dont use in real vesrion
            mTruck->shoot(mAssignedModelNode->getPosition().vec3d(), 20000);//1000 m/s
            mMapController->setTrackNode(mFiredRocket->getGeoTransform());
        }
    }
}

void SystemModelNode::frameEvent()
{
    //--update lable position---------------------------------------------------
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
    //--update assigned line----------------------------------------------------
    if(mAssignedModelNode)
    {
        mAssignedLine->clearPoints();
        mAssignedLine->addPoint(getPosition().vec3d());
        mAssignedLine->addPoint(mAssignedModelNode->getPosition().vec3d());
    }
    //--check collision--------------------------------------------------------
    collision();
}

void SystemModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    //    qDebug()<<"type:"<<event->type();
    //    BaseModel::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
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
        mMapController->addNode(mRangeCircle);
    }
    else
    {
        mMapController->removeNode(mRangeCircle);
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

        //        mMapController->addNode(mWezPolygon);
        mMapController->getMapNode()->insertChild(0,mWezPolygon);

    }
    else
        mMapController->removeNode(mWezPolygon);
}

void SystemModelNode::onMezButtonToggled(bool checked)
{
    if(checked)
    {
        mMezSphere->setPosition(getPosition());
        mMezSphere->setRadius(mInformation.MezRange);
        mMapController->addNode(mMezSphere);
    }
    else
    {
        mMapController->removeNode(mMezSphere);
    }
}

void SystemModelNode::onActiveButtonToggled(bool checked)
{
    mInformation.Active = checked;
}
