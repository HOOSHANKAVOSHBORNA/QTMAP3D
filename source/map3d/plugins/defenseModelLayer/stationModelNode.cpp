#include "stationModelNode.h"
#include "systemModelNode.h"
#include "truck.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osg/Material>
#include "defenseModelNodeAutoScaler.h"

const float RANGE3D = std::numeric_limits<float>::max();

osg::ref_ptr<osg::Node> StationModelNode::mNode3DRef;

StationModelNode::StationModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    mIs3D = mMapController->getMode();

    mRootNode = new osg::LOD;
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);

    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler(2.5, 1, 1000));



    setStyle(rootStyle);


    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);


    osg::ref_ptr<osg::Image> mainImage = osgDB::readImageFile("../data/models/station/station.png");


    osg::ref_ptr<osg::Image> stationImageActive = createColoredImage(mainImage, osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f));
    if(stationImageActive)
        stationImageActive->scaleImage(100, 100, stationImageActive->r());
    osg::ref_ptr<osg::Geometry> stationImageDrawableActive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageActive, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeActive = new osg::Geode();
    stationGeodeActive->setStateSet(geodeStateSet);
    stationGeodeActive->addDrawable(stationImageDrawableActive);


    osg::ref_ptr<osg::Image> stationImageActiveHovered = createDarkerImage(stationImageActive, 0.5f);
    if(stationImageActiveHovered)
        stationImageActiveHovered->scaleImage(100, 100, stationImageActiveHovered->r());
    osg::ref_ptr<osg::Geometry> stationImageDrawableActiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageActiveHovered, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeActiveHovered = new osg::Geode();
    stationGeodeActiveHovered->setStateSet(geodeStateSet);
    stationGeodeActiveHovered->addDrawable(stationImageDrawableActiveHovered);

    osg::ref_ptr<osg::Image> stationImageDeactive = createColoredImage(mainImage, osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f));
    if(stationImageDeactive)
        stationImageDeactive->scaleImage(100, 100, stationImageDeactive->r());
    osg::ref_ptr<osg::Geometry> stationImageDrawableDeactive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageDeactive, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeDeactive = new osg::Geode();
    stationGeodeDeactive->setStateSet(geodeStateSet);
    stationGeodeDeactive->addDrawable(stationImageDrawableDeactive);


    osg::ref_ptr<osg::Image> stationImageDeactiveHovered = createDarkerImage(stationImageDeactive, 0.5f);
    if(stationImageDeactiveHovered)
        stationImageDeactiveHovered->scaleImage(100, 100, stationImageDeactiveHovered->r());
    osg::ref_ptr<osg::Geometry> stationImageDrawableDeactiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageDeactiveHovered, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeDeactiveHovered = new osg::Geode();
    stationGeodeDeactiveHovered->setStateSet(geodeStateSet);
    stationGeodeDeactiveHovered->addDrawable(stationImageDrawableDeactiveHovered);


    osg::AutoTransform *at = new osg::AutoTransform;


    mNode2DActive = new osg::Switch;
    mNode2DActive->addChild(stationGeodeActive, true);
    mNode2DActive->addChild(stationGeodeActiveHovered, false);

    mNode2DDeactive = new osg::Switch;
    mNode2DDeactive->addChild(stationGeodeDeactive, true);
    mNode2DDeactive->addChild(stationGeodeDeactiveHovered, false);

    mNode2D = new osg::Switch;
    mNode2D->addChild(mNode2DActive, true);
    mNode2D->addChild(mNode2DDeactive, false);

    at->addChild(mNode2D);
    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);


    if (!mNode3DRef.valid()) {
        mNode3DRef = osgDB::readRefNodeFile("../data/models/station/Station.osgb");
    }
    if (!mNode3DRef)
    {
        return;
    }

    mNode3D = new osg::Group;
    mNode3D->addChild(mNode3DRef.get());


    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;

    updateOrCreateLabelImage();
    mLabelNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);

    getGeoTransform()->addChild(mLabelNode);
    mLabelNode->setNodeMask(false);

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


    connect(mapControler, &MapController::modeChanged, this, &StationModelNode::onModeChanged);

    mRangeCircle = new Circle(mMapController, true);
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));

    mVisiblePolygone = new Polygone(mMapController, true);
    mVisiblePolygone->setLineColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));
    mVisiblePolygone->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.5f));
}

void StationModelNode::setInformation(const StationInfo& info)
{
    mInformation = info;
    updateOrCreateLabelImage();

    mNode2D->setValue(0, info.Active);
    mNode2D->setValue(1, !info.Active);

}
void StationModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform(), 250);
}

void StationModelNode::onLeftButtonClicked(bool val)
{
    if(val && !mIsSelected)
    {
        showInfoWidget();
    }
    if(!val)
    {
        mMapController->untrackNode(getGeoTransform());
        onRangeButtonToggled(val);
        onVisibleButtonToggled(val);
    }
    select(val);
}

void StationModelNode::frameEvent()
{
    mLabelNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));
}

void StationModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
}

void StationModelNode::hover(bool val)
{
    DefenseModelNode::hover(val);

    mNode2DActive->setValue(0, !val);
    mNode2DActive->setValue(1, val);

    mNode2DDeactive->setValue(0, !val);
    mNode2DDeactive->setValue(1, val);
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

        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->addChild(mRangeCircle);
            }
        }
    }
    else
    {
        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->removeChild(mRangeCircle);
            }
        }
    }
}

void StationModelNode::onVisibleButtonToggled(bool checked)
{
    if(checked)
    {
        mVisiblePolygone->clearPoints();
        osg::Vec3d worldPosition;
        getPosition().toWorld(worldPosition, mMapController->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint;
        double radius = mInformation.Radius;
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() - radius*2/3, worldPosition.y() - radius*2/3, worldPosition.z()));
        geoPoint.z() = 0;
        mVisiblePolygone->addPoints(geoPoint.vec3d());
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() - radius*2/3, worldPosition.y() + radius*2/3, worldPosition.z()));
        geoPoint.z() = 0;
        mVisiblePolygone->addPoints(geoPoint.vec3d());
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() + radius*2/3, worldPosition.y() + radius*2/3, worldPosition.z()));
        geoPoint.z() = 0;
        mVisiblePolygone->addPoints(geoPoint.vec3d());
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() + radius*2/3, worldPosition.y() - radius*2/3, worldPosition.z()));
        geoPoint.z() = 0;
        mVisiblePolygone->addPoints(geoPoint.vec3d());

        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->addChild(mVisiblePolygone);
            }
        }
    }
    else
    {
        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->removeChild(mVisiblePolygone);
            }
        }
    }
}

void StationModelNode::onActivateButtonToggled(bool checked)
{
    mInformation.Active = checked;

    mNode2D->setValue(0, checked);
    mNode2D->setValue(1, !checked);


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
    connect(stationInformation->getInfo(), &StationInfoModel::activeButtonToggled, this, &StationModelNode::onActivateButtonToggled);
    stationInformation->show();
}

void StationModelNode::updateOrCreateLabelImage()
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
                         "CycleTime:");
        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(mInformation.CycleTime));


        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "PrimSec:");
        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mInformation.PrimSec);



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
