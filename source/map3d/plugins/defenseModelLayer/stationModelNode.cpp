#include "stationModelNode.h"
#include "systemModelNode.h"
#include "truck.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osg/Material>

const float RANGE3D = std::numeric_limits<float>::max();

class StationModelNodeAutoScaler : public osg::NodeCallback
{
public:
    StationModelNodeAutoScaler(const osg::Vec3d& baseScale = osg::Vec3d(1,1,1), double minScale = 0.0, double maxScale = DBL_MAX) :
        _baseScale( baseScale ),
        _minScale( minScale ),
        _maxScale( maxScale )
    {

    }

public: // osg::NodeCallback

    void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgEarth::Annotation::GeoPositionNode* geo = static_cast<osgEarth::Annotation::GeoPositionNode*>(node);
        osgUtil::CullVisitor* cs = static_cast<osgUtil::CullVisitor*>(nv);

        osg::Camera* cam = cs->getCurrentCamera();

        // If this is an RTT camera, we need to use it's "parent"
        // to calculate the proper scale factor.
        if (cam->isRenderToTextureCamera() &&
                cam->getView() &&
                cam->getView()->getCamera() &&
                cam->getView()->getCamera() != cam)
        {
            cam = cam->getView()->getCamera();
        }

        if (cam->getViewport())
        {
            // Reset the scale so we get a proper bound
            geo->getPositionAttitudeTransform()->setScale(_baseScale);
            const osg::BoundingSphere& bs = node->getBound();

            // transform centroid to VIEW space:
            osg::Vec3d centerView = bs.center() * cam->getViewMatrix();

            // Set X coordinate to the radius so we can use the resulting CLIP
            // distance to calculate meters per pixel:
            centerView.x() = bs.radius();

            // transform the CLIP space:
            osg::Vec3d centerClip = centerView * cam->getProjectionMatrix();

            // caluclate meters per pixel:
            double mpp = (centerClip.x()*0.5) * cam->getViewport()->width();

            // and the resulting scale we need to auto-scale.
            double scale = bs.radius() / mpp;

            scale *= 3.5;

            if (scale < _minScale)
                scale = _minScale;
            else if (scale>_maxScale)
                scale = _maxScale;

            geo->getPositionAttitudeTransform()->setScale(
                        osg::componentMultiply(_baseScale, osg::Vec3d(scale, scale, scale)));
        }

        if (node->getCullingActive() == false)
        {
            node->setCullingActive(true);
        }

        traverse(node, nv);
    }

protected:
    osg::Vec3d _baseScale;
    double _minScale;
    double _maxScale;
};


osg::ref_ptr<osg::Node> StationModelNode::mNode3DRef;

StationModelNode::StationModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    mIs3D = mMapController->getMode();
    //--create root node---------------------------------------------------------------------------
    mRootNode = new osg::LOD;
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = false;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 1200 * 3.5;

    //--auto scaler---------------------------------------------------------------------------------
    mModelNodeAutoScaler = new StationModelNodeAutoScaler( osg::Vec3d(1,1,1),
                                                           rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale().value(),
                                                           rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale().value() );
    this->setCullingActive(false);
    this->addCullCallback(mModelNodeAutoScaler);
    //----------------------------------------------------------------------------------------------
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    setStyle(rootStyle);
    //--create 2d node----------------------------------------------------------------------------
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

    osg::Image* redIcon = osgDB::readImageFile("../data/models/station/station_ll_red.png");
    if(redIcon)
        redIcon->scaleImage(20, 20, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();

    redGeode->setStateSet(geodeStateSet);
    redGeode->addDrawable(redImageDrawable);
    //    auto redPlaceNode = new osgEarth::Annotation::PlaceNode();
    //    redPlaceNode->setIconImage(redIcon);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/station/station_ll_yell.png");
    if(yellowIcon)
        yellowIcon->scaleImage(20, 20, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->setStateSet(geodeStateSet);
    yellowGeode->addDrawable(yellowImageDrawable);
    //    auto yellowPlaceNode = new osgEarth::Annotation::PlaceNode();
    //    yellowPlaceNode->setIconImage(yellowIcon);

    mNode2D = new osg::Switch;
    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);
    //    mNode2D->addChild(yellowPlaceNode->getGeoTransform(), false);
    //    mNode2D->addChild(redPlaceNode->getGeoTransform(), true);
    //    getGeoTransform()->addChild(redPlaceNode);

    //--create 3D node---------------------------------------------------------------------------
    if (!mNode3DRef.valid()) {
        mNode3DRef = osgDB::readRefNodeFile("../data/models/station/station.ive");
    }
    if (!mNode3DRef)
    {
        return;
    }

    mNode3D = new osg::Group;
    mNode3D->addChild(mNode3DRef.get());
    //    truck->setQStringName("truck");
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    //osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    updateOrCreateLabelImage();
    mLableNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);
    //    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0.5, 1));
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
    mVisiblePolygone->setLineColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));
    mVisiblePolygone->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.5f));
    //    setDefaultLighting( false );
}

void StationModelNode::setInformation(const StationInfo& info)
{
    mInformation = info;
    updateOrCreateLabelImage();
}
void StationModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
    //mMapController->goToPosition(getPosition(), 200);
}

void StationModelNode::onLeftButtonClicked(bool val)
{
    if(val && !mIsSelected)
    {
        showInfoWidget();
    }
    if(!val)
    {
        mMapController->untrackNode();
        onRangeButtonToggled(val);
        onVisibleButtonToggled(val);
    }
    select(val);
}

void StationModelNode::frameEvent()
{
    //    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));
}

void StationModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
    //BaseModel::mousePressEvent(event, onModel);
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
        //mMapController->addNode(mRangeCircle);

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
        //mMapController->removeNode(mRangeCircle);
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

        //mVisiblePolygone->setHeight(0);
        //mMapController->addNode(mVisiblePolygone);
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
        //mMapController->removeNode(mVisiblePolygone);
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

        //        painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH-20, 30),
        //                         Qt::AlignLeft | Qt::AlignVCenter,
        //                         "I-Method:");
        //        painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH-20, 30),
        //                         Qt::AlignRight | Qt::AlignVCenter,
        //                         mInformation.IdentificationMethod);


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
