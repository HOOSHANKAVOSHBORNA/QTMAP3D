﻿#include "stationModelNode.h"
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osg/Material>
#include <QPainter>
#include "defenseModelNodeAutoScaler.h"
#include "stationDataManager.h"

const float RANGE3D = 835;// std::numeric_limits<float>::max();

osg::ref_ptr<osg::Node> StationModelNode::mNode3DRef;


StationModelNode::StationModelNode(DefenseModelLayer *defenseModelLayer, Station::Data *data, QObject *parent)
    :DefenseModelNode(defenseModelLayer->mapItem(), parent),
	  mDefenseModelLayer(defenseModelLayer),
      mData(data)
{
    connect(mDefenseModelLayer->mapItem(), &MapItem::modeChanged, this, &StationModelNode::onModeChanged);

    mIs3D = mDefenseModelLayer->mapItem()->getMode();

    mRootNode = new osg::LOD;
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);

    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler(2.5, 1, 1000));



    setStyle(rootStyle);


    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);


    static bool bFirst = true;
    static osg::ref_ptr<osg::Image> mainImage;
    static osg::ref_ptr<osg::Image> stationImageActive;
    static osg::ref_ptr<osg::Image> stationImageActiveHovered;
    static osg::ref_ptr<osg::Image> stationImageDeactive;
    static osg::ref_ptr<osg::Image> stationImageDeactiveHovered;


    if (bFirst) {
        mainImage = osgDB::readImageFile("../data/models/station/station.png");

        stationImageActive = createColoredImage(mainImage, osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f));
        stationImageActiveHovered = createDarkerImage(stationImageActive, 0.5f);
        stationImageDeactive = createColoredImage(mainImage, osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f));
        stationImageDeactiveHovered = createDarkerImage(stationImageDeactive, 0.5f);


        if(stationImageActive)
            stationImageActive->scaleImage(100, 100, stationImageActive->r());
        if(stationImageActiveHovered)
            stationImageActiveHovered->scaleImage(100, 100, stationImageActiveHovered->r());
        if(stationImageDeactive)
            stationImageDeactive->scaleImage(100, 100, stationImageDeactive->r());
        if(stationImageDeactiveHovered)
            stationImageDeactiveHovered->scaleImage(100, 100, stationImageDeactiveHovered->r());

        bFirst = false;
    }

    osg::ref_ptr<osg::Geometry> stationImageDrawableActive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageActive, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeActive = new osg::Geode();
    stationGeodeActive->setStateSet(geodeStateSet);
    stationGeodeActive->addDrawable(stationImageDrawableActive);


    osg::ref_ptr<osg::Geometry> stationImageDrawableActiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageActiveHovered, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeActiveHovered = new osg::Geode();
    stationGeodeActiveHovered->setStateSet(geodeStateSet);
    stationGeodeActiveHovered->addDrawable(stationImageDrawableActiveHovered);

    osg::ref_ptr<osg::Geometry> stationImageDrawableDeactive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(stationImageDeactive, osg::Vec2s(0,0), 0, 0, 0.2);
    osg::ref_ptr<osg::Geode>  stationGeodeDeactive = new osg::Geode();
    stationGeodeDeactive->setStateSet(geodeStateSet);
    stationGeodeDeactive->addDrawable(stationImageDrawableDeactive);


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

    mNode3D = new osg::Group;
    mNode3D->addChild(mNode3DRef.get());


    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;

    updateOrCreateLabelImage();
    mStatusNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);

    getGeoTransform()->addChild(mStatusNode);
    mStatusNode->setNodeMask(false);
    mStatusNode->setPriority(10);

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

    mBackCircleNode = new Circle();
	mBackCircleNode->setRadius(6.5);
	mBackCircleNode->setColor(osgEarth::Color(0.2f, 0.2f, 0.2f, 0.05f));
	mBackCircleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.08));

//    mCircleStyleActive.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f);
//    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
//    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
//    mCircleStyleActive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

//    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f);
//    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
//    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
//    mCircleStyleDeactive.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;


//    mCircleNode->setStyle(mCircleStyleActive);
//    mCircleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.05));
//    mCircleNode->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);

	mNode3D->addChild(mBackCircleNode);

    mRangeCircle = new Circle();
	mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.3f));
	mRangeCircle->setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping::CLAMP_TO_TERRAIN);


    mVisiblePolygon = new Polygon(mDefenseModelLayer->mapItem());
	mVisiblePolygon->setLineColor(osg::Vec4(1.0, 0.0, 0.0, 0.3f));
	mVisiblePolygon->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));
    mVisiblePolygon->setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping::CLAMP_TO_TERRAIN);
}

void StationModelNode::dataChanged()
{
    updateOrCreateLabelImage();

	mNode2D->setValue(0, mData->info.RadarSearchStatus == StationInfo::S);
	mNode2D->setValue(1, mData->info.RadarSearchStatus != StationInfo::S);

	updateColors();

}

bool StationModelNode::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    mStatusNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));
    return false;
}

bool StationModelNode::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, bool onModel)
{
    return DefenseModelNode::mousePressEvent(ea, aa, onModel);
}

void StationModelNode::updateColors()
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
	if (mData->info.RadarSearchStatus == StationInfo::S) {
		mModelColor = osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f);
	} else {
		mModelColor = osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f);
	}

	DefenseModelNode::updateColors();

}

void StationModelNode::setSelectionMode(DefenseModelNode::SelectionMode sm)
{
	DefenseModelNode::setSelectionMode(sm);

	if(sm == SELECTED)
	{
		showInfoWidget();
	}
	else
	{
//		mDefenseModelLayer->mapItem()->untrackNode(getGeoTransform());
//        onRangeButtonToggled(val);
//        onVisibleButtonToggled(val);
        if (mStationInformation)
            mStationInformation->hide();
	}
}

void StationModelNode::onGotoButtonClicked()
{
    mDefenseModelLayer->mapItem()->getCameraController()->goToPosition(getPosition(), 200);
}

void StationModelNode::onRangeButtonToggled(bool check)
{
    if(check)
    {
        mRangeCircle->setPosition(getPosition());
		mRangeCircle->setRadius(osgEarth::Distance(mData->info.Radius, osgEarth::Units::METERS));

//        auto layer = mDefenseModelLayer->mapItem()->getMapNode()->getMap()->getLayerByName(STATION_LAYER);
//        if (layer) {
//            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//            if (group) {
//                group->addChild(mRangeCircle);
//            }
//        }
        mDefenseModelLayer->getModelLayer(STATION_LAYER)->addChild(mRangeCircle);
    }
    else
    {
//        auto layer = mDefenseModelLayer->mapItem()->getMapNode()->getMap()->getLayerByName(STATION_LAYER);
//        if (layer) {
//            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//            if (group) {
//                group->removeChild(mRangeCircle);
//            }
//        }
        mDefenseModelLayer->getModelLayer(STATION_LAYER)->removeChild(mRangeCircle);
    }
}

void StationModelNode::onVisibleButtonToggled(bool checked)
{
    if(checked)
    {
        if(mVisiblePolygon->getSize() <=0)
        {
        mVisiblePolygon->clearPoints();
        osg::Vec3d worldPosition;
        getPosition().toWorld(worldPosition, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());
        osgEarth::GeoPoint geoPoint;
		double radius = mData->info.Radius;
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() - radius*2/3, worldPosition.y() - radius*2/3, worldPosition.z()));
        //geoPoint.z() = 0;
        mVisiblePolygon->addPoints(geoPoint);
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() - radius*2/3, worldPosition.y() + radius*2/3, worldPosition.z()));
        //geoPoint.z() = 0;
        mVisiblePolygon->addPoints(geoPoint);
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() + radius*2/3, worldPosition.y() + radius*2/3, worldPosition.z()));
        //geoPoint.z() = 0;
        mVisiblePolygon->addPoints(geoPoint);
        geoPoint.fromWorld(getPosition().getSRS(), osg::Vec3d(worldPosition.x() + radius*2/3, worldPosition.y() - radius*2/3, worldPosition.z()));
        //geoPoint.z() = 0;
        mVisiblePolygon->addPoints(geoPoint);
        }

        mDefenseModelLayer->getModelLayer(STATION_LAYER)->addChild(mVisiblePolygon);
//        mDefenseModelLayer->mapItem()->addNodeToLayer(mVisiblePolygon, STATION_LAYER);
    }
    else
    {
        mDefenseModelLayer->getModelLayer(STATION_LAYER)->removeChild(mVisiblePolygon);
//        mDefenseModelLayer->mapItem()->removeNodeFromLayer(mVisiblePolygon, STATION_LAYER);
    }
}

void StationModelNode::onActivateButtonToggled(bool checked)
{
	mData->info.RadarSearchStatus = (checked ?  StationInfo::S : StationInfo::US);

    mNode2D->setValue(0, checked);
    mNode2D->setValue(1, !checked);


//    mCircleNode->setStyle(checked ? mCircleStyleActive : mCircleStyleDeactive);
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

    //select(mIsSelected);
    updateColors();
}

void StationModelNode::showInfoWidget()
{
    if (mStationInformation == nullptr) {
		mStationInformation = new StationInformtion(mDefenseModelLayer,mData, this);
        connect(mStationInformation->getInfo(), &StationInfoModel::gotoButtonClicked, this, &StationModelNode::onGotoButtonClicked);
        connect(mStationInformation->getInfo(), &StationInfoModel::rangeButtonClicked, this, &StationModelNode::onRangeButtonToggled);
        connect(mStationInformation->getInfo(), &StationInfoModel::visibleButtonClicked, this, &StationModelNode::onVisibleButtonToggled);
        connect(mStationInformation->getInfo(), &StationInfoModel::activeButtonToggled, this, &StationModelNode::onActivateButtonToggled);
    }
    mStationInformation->show();
}

void StationModelNode::updateOrCreateLabelImage()
{
	int txtLeftPos = 5;
	int txtTopPos = 0;
	//	int txtWidth = 30;
	int txtHeight = 22;

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

		static const QFont textFont("SourceSansPro", 10, QFont::Normal);
		static const QFont textFontBold("SourceSansPro", 10, QFont::Bold);
        static const QPen  textPen(QColor(255, 255, 255));
		static QPen  textPenYellow(QColor(204, 204, 51, 255));

        painter.setPen(Qt::NoPen);
        painter.setBrush(backgroundBrush);

        painter.drawRoundedRect(
                    mRenderTargetImage->rect(),
                    8,8);
        painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
        painter.drawRoundedRect(
					QRect(0, 0, LABEL_IMAGE_WIDTH, txtHeight),
                    8,8);
        //-----------------------------------------------------------------
        static const QPen linePen(QColor(255, 255, 255),
                                  1,
                                  Qt::PenStyle::DashLine
                                  );

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, txtHeight, LABEL_IMAGE_WIDTH, txtHeight);

		painter.drawLine(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, txtHeight);

		painter.setPen(textPenYellow);
		painter.setFont(textFontBold);
		painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignCenter,
						 mData->info.Name);

		painter.drawText(QRect(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignCenter,
						 QString::number(mData->info.Number));
        //----------------------------------------------------------------
		painter.setPen(textPen);
		painter.setFont(textFont);
		txtTopPos += txtHeight;
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft| Qt::AlignVCenter,
                         "PrimSec:");
		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.PrimSec);

		txtTopPos += txtHeight;
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Type:");
		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.Type);

		txtTopPos += txtHeight;
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "Radius:");
		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
						 QString::number(mData->info.Radius/1000.0, 'f', 1) + " km");



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
