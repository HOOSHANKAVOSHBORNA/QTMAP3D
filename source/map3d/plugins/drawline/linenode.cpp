#include "linenode.h"
#include "drawshapeautoscaler.h"
#include <osgEarth/GeoMath>
#include <osg/Point>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QDialog>
#include <osgEarthSymbology/Geometry>
#include <osgEarth/ElevationQuery>


LineNode::LineNode(MapController *mapController)
{
	mMapController = mapController;
	mLineGeometry = new osgEarth::Symbology::Geometry();
	osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLineGeometry, mMapController->getMapSRS());
	pathFeature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

	osgEarth::Symbology::Style pathStyle;
	//    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
	//    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
	//    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;

	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mColor;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = mTessellation;

	_options = osgEarth::Features::GeometryCompilerOptions();
	_needsRebuild = true;
	_styleSheet = nullptr;
	_clampDirty = false;
	_index = nullptr;

	setFeature(pathFeature);
	setStyle(pathStyle);
	mLabelGroup = new osg::Group;
	addChild(mLabelGroup);
	mLabelGroup->setNodeMask(false);
}

LineNode::~LineNode()
{
	for(auto labelData: mVecLabelData){
		if(labelData.qImage)
			delete labelData.qImage;
	}
}

void LineNode::addPoint(osgEarth::GeoPoint point)
{
	mLineGeometry->push_back(point.vec3d());
	dirty();

	if(getSize() >= 2)
	{
		std::vector<osg::Vec3d> distanceVectorPoint;
		distanceVectorPoint.push_back(mLineGeometry->at(mLineGeometry->size() - 2));
		distanceVectorPoint.push_back(mLineGeometry->at(mLineGeometry->size() - 1));
		auto lenght = osgEarth::GeoMath().rhumbDistance(distanceVectorPoint);

		double bearingRadian = osgEarth::GeoMath().bearing(osg::DegreesToRadians(mLineGeometry->at(mLineGeometry->size() - 2).y()),
														   osg::DegreesToRadians(mLineGeometry->at(mLineGeometry->size() - 2).x()),
														   osg::DegreesToRadians(mLineGeometry->at(mLineGeometry->size() - 1).y()),
														   osg::DegreesToRadians(mLineGeometry->at(mLineGeometry->size() - 1).x()));
		auto bearing = osg::RadiansToDegrees(bearingRadian);
		if (bearing < 0){
			bearing += 360;
		}
        // Calculate the slope between the two points.
        osgEarth::GeoPoint p1(mMapController->getMapSRS() ,mLineGeometry->at(mLineGeometry->size() - 2).x(),
                              mLineGeometry->at(mLineGeometry->size() - 2).y()
                              , mLineGeometry->at(mLineGeometry->size() - 2).z());
        osgEarth::GeoPoint p2(mMapController->getMapSRS(),mLineGeometry->at(mLineGeometry->size() - 1).x(),
                              mLineGeometry->at(mLineGeometry->size() - 1).y(),
                              mLineGeometry->at(mLineGeometry->size() - 1).z());
        double slope = (mLineGeometry->at(mLineGeometry->size() - 1).z() - mLineGeometry->at(mLineGeometry->size() - 2).z()) / p1.distanceTo(p2);
        slope = std::asin(slope);
        slope = osg::RadiansToDegrees(slope);

//		QImage** image = new QImage*;
//		*image = nullptr;
		osg::ref_ptr<osg::Image> image = new osg::Image;
		QImage* qImage = createOrUpdateLabelImg(image, lenght, bearing);
		osg::ref_ptr<osgEarth::Annotation::PlaceNode> placeNode = new osgEarth::Annotation::PlaceNode();

		LabelData data;
		data.qImage = qImage;
		data.image = image;
		data.lenght = lenght;
		data.bearing = bearing;
        data.slope = slope;
		data.placeNode = placeNode;
		mVecLabelData.push_back(std::move(data));

		//        if(!(mShowLenght||mShowBearing)){
		//            mLabelGroup->setNodeMask(false);
		//            mPlaceNode->setStyle(mPlaceNode->getStyle());
		//        }
		placeNode->setIconImage(image);

		osgEarth::GeoPoint midPoint(mMapController->getMapSRS(),
									(mLineGeometry->at(mLineGeometry->size() - 2) + mLineGeometry->at(mLineGeometry->size() -1 )) / 2);
		placeNode->setPosition(midPoint);
		placeNode->setStyle(placeNode->getStyle());
		mLabelGroup->addChild(placeNode);
		//mLabelGroup->setNodeMask(true);
		//        addChild(mLabelGroup);
	}
	addChild(mLabelGroup);
}

void LineNode::removePoint()
{
	mLineGeometry->pop_back();
	auto qImage = mVecLabelData[mVecLabelData.size() - 1].qImage;
	if(qImage)
		delete  qImage;

	mVecLabelData.pop_back();
	dirty();
	mLabelGroup->removeChild(getSize()-1);

	addChild(mLabelGroup);
}

void LineNode::removeFirstPoint()
{
	mLineGeometry->erase(mLineGeometry->begin());
	auto qImage = mVecLabelData[0].qImage;
	if(qImage)
		delete  qImage;
	mVecLabelData.erase(mVecLabelData.begin());
	dirty();
	mLabelGroup->removeChild(0, 1);

	addChild(mLabelGroup);
}

void LineNode::clear()
{
	mLineGeometry->clear();
	for(auto labelData: mVecLabelData){
		if(labelData.qImage)
			delete labelData.qImage;
	}
	mVecLabelData.clear();
	dirty();
	mLabelGroup->removeChildren(0,mLabelGroup->getNumChildren());

	addChild(mLabelGroup);
}

int LineNode::getSize()
{
	return static_cast<int>(mLineGeometry->size());
}

osgEarth::Color LineNode::getColor() const
{
	return mColor;
}

void LineNode::setColor(const osgEarth::Color &color)
{
	if(mColor == color)
		return;
	mColor = color;
	auto style = getStyle();
	style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color()= mColor;
	setStyle(style);
	addChild(mLabelGroup);
}

float LineNode::getWidth() const
{
	return mWidth;
}

void LineNode::setWidth(float width)
{
	mWidth = width;
	auto style = getStyle();
	style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
	setStyle(style);
	addChild(mLabelGroup);
}

float LineNode::getHeight() const
{
	return mHeight;
}

void LineNode::setHeight(float height)
{
	mHeight = height;
	auto style = getStyle();
	style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = mHeight;
	if (height<=0){
		style.remove<osgEarth::Symbology::ExtrusionSymbol>();
	}
	setStyle(style);
	addChild(mLabelGroup);
}

osgEarth::Symbology::AltitudeSymbol::Clamping LineNode::getClamp() const
{
	return mClamp;
}

void LineNode::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp)
{
	if(mClamp == clamp)
		return;

	mClamp = clamp;
	auto style = this->getStyle();
	style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = clamp;
	setStyle(style);
	addChild(mLabelGroup);
}

unsigned LineNode::getTessellation() const
{
	return mTessellation;
}

void LineNode::setTessellation(const unsigned &tessellation)
{
	if(mTessellation == tessellation)
		return;
	mTessellation = tessellation;
	auto style = this->getStyle();
	style.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = tessellation;
	setStyle(style);
	addChild(mLabelGroup);
}

//--------------------------------------------------------------
bool LineNode::getPointVisible() const
{
	return mPointVisible;
}

void LineNode::setPointVisible(bool value)
{
	if(mPointVisible == value)
		return;

	mPointVisible = value;
	auto style = getStyle();
	if(mPointVisible)
	{
		style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
		style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
		style.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;
	}
	else
		style.remove<osgEarth::Symbology::PointSymbol>();
	setStyle(style);
	addChild(mLabelGroup);
}

bool LineNode::getShowBearing() const
{
	return mShowBearing;
}

void LineNode::setShowBearing(const bool &bearing)
{
	mShowBearing = bearing;
	for(auto& data:mVecLabelData)
	{
		createOrUpdateLabelImg(data.image, data.lenght, data.bearing);
		data.placeNode->setStyle(data.placeNode->getStyle());
	}
	if(!(mShowDistance||mShowBearing)){
		mLabelGroup->setNodeMask(false);
	}
	else{
		mLabelGroup->setNodeMask(true);
	}

}

void LineNode::setShowDistance(const bool &show)
{
	mShowDistance = show;
	for(auto& data:mVecLabelData)
	{
		createOrUpdateLabelImg(data.image, data.lenght, data.bearing);
		data.placeNode->setStyle(data.placeNode->getStyle());
	}
	if(!(mShowDistance||mShowBearing)){
		mLabelGroup->setNodeMask(false);
	}
	else{
		mLabelGroup->setNodeMask(true);
	}

}

bool LineNode::getShowDistance() const
{
	return  mShowDistance;
}

osgEarth::Color LineNode::getPointColor() const
{
	return mPointColor;
}

void LineNode::setPointColor(const osgEarth::Color &pointColor)
{
	if(mPointColor == pointColor)
		return;
	mPointColor = pointColor;
	if(mPointVisible)
	{
		auto style = getStyle();
		style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
		setStyle(style);
		addChild(mLabelGroup);
	}
}

float LineNode::getPointWidth() const
{
	return mPointWidth;
}

void LineNode::setPointWidth(float pointWidth)
{
	mPointWidth = pointWidth;
	auto style = getStyle();
	if(mPointVisible)
	{
		style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
		setStyle(style);
		addChild(mLabelGroup);
	}
}

bool LineNode::getSmooth() const
{
	return mSmooth;
}

void LineNode::setSmooth(bool smooth)
{
	mSmooth = smooth;
	auto sStyle = getStyle();
	if(mPointVisible)
	{
		sStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;
		setStyle(sStyle);
		addChild(mLabelGroup);

	}
}
//---------------------------------------------------------------
QImage *LineNode::createOrUpdateLabelImg(osg::ref_ptr<osg::Image>& image, double lenght, double bearing)
{
	int imageHeight = LABEL_IMAGE_HEIGHT;
	int bearingPos = 8;
	if (mShowDistance && mShowBearing){
		imageHeight = LABEL_IMAGE_HEIGHT + 20 ;
		bearingPos = 30;
	}


	QImage* lblImage = new QImage(
				LABEL_IMAGE_WIDTH,
				imageHeight,
				QImage::Format_RGBA8888);

//	QImage *lblImage = *qImage;

//	if (!lblImage) {
//		lblImage = new QImage(
//					LABEL_IMAGE_WIDTH,
//					mHeightLbl,
//					QImage::Format_RGBA8888);
//	} else {
//		if (lblImage->width() != LABEL_IMAGE_WIDTH ||
//				lblImage->height() != mHeightLbl) {

//			lblImage->~QImage();
//			new(lblImage) QImage(
//						LABEL_IMAGE_WIDTH,
//						mHeightLbl,
//						QImage::Format_RGBA8888);
//		}
//	}
//	*qImage = lblImage;

	if(!image->valid())
		image = new osg::Image;
	{

		lblImage->fill(QColor(Qt::red));
		QPainter painter(lblImage);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

		static const QBrush backgroundBrush = QBrush(QColor(0, 0, 0, int(255 * 0.3f)));
		static const QFont textFont("SourceSansPro", 10, QFont::Normal);
		static const QPen  textPen(QColor(255, 255, 255));

		painter.setBrush(backgroundBrush);
		painter.drawRoundedRect(
					lblImage->rect(),
					10,2);


		painter.setPen(textPen);
		painter.setFont(textFont);

		if (mShowDistance){
			if (lenght >= 1000){
				lenght/=1000;
				QString str = QObject::tr("d: %1 km").arg(lenght,0,'f',2);
				painter.drawText(4, 10, LABEL_IMAGE_WIDTH, 15,
								 Qt::AlignLeft|Qt::AlignVCenter,
								 str);
			}
			else{
				QString str = QObject::tr("d: %1 m").arg(lenght,0,'f',2);
				painter.drawText(4, 10, LABEL_IMAGE_WIDTH, 15,
								 Qt::AlignLeft|Qt::AlignVCenter,
								 str);
			}
		}
		if (mShowBearing){
//			QString bearStr= QString::number(bearing, 'f', 2);
			QString bearStr= QObject::tr("b: %1°").arg(bearing,0,'f',2);
			painter.drawText(QRect(4, bearingPos, LABEL_IMAGE_WIDTH, 15),
							 Qt::AlignLeft|Qt::AlignVCenter,
							 bearStr);


		}

	}
	*lblImage = lblImage->mirrored(false, true);
	//    QDialog *dlg = new QDialog();
	//    QLabel *label = new QLabel;
	//    label->setPixmap(QPixmap::fromImage(*lblImage));
	//    QVBoxLayout *lay = new QVBoxLayout;
	//    lay->addWidget(label);
	//    lay->setStretch(0, 1000);
	//    dlg->setLayout(lay);

	//    dlg->exec();


	image->setImage(LABEL_IMAGE_WIDTH,
				  imageHeight,
				  1,
				  GL_RGBA,
				  GL_RGBA,
				  GL_UNSIGNED_BYTE,
				  lblImage->bits(),
				  osg::Image::AllocationMode::NO_DELETE);
	return lblImage;

}

bool LineNode::getShowSlope() const
{
    return mShowSlope;
}

void LineNode::setShowSlope(bool showSlope)
{
    mShowSlope = showSlope;
    for(auto& data:mVecLabelData)
    {
//        createOrUpdateLabelImg(data.qImage, data.image, data.lenght, data.bearing );
        data.placeNode->setStyle(data.placeNode->getStyle());
    }
//    if(mPlaceNode){
//        if(!(mShowLenght||mShowBearing||mShowSlope)){
//            mLabelGroup->setNodeMask(false);
//            mPlaceNode->setStyle(mPlaceNode->getStyle());
//        }
//        else{
//            mLabelGroup->setNodeMask(true);
//            mPlaceNode->setStyle(mPlaceNode->getStyle());
//        }
//    }
}
