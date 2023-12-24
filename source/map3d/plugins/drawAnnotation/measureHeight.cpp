#include "measureHeight.h"

MeasureHeight::MeasureHeight(MapItem *mapItem)
{
    mMapItem = mapItem;
    mHLine = new LineNode(mMapItem);
    mVLine = new LineNode(mMapItem);
    mHLine->setFillColor(osgEarth::Color::White);
    mVLine->setWidth(5);
    mHLine->setWidth(5);
	mHLine->setShowDistance(false);
//    mHLine->setIsHeight(true);
	mVLine->setShowDistance(false);
    mVLine->setPointVisible(false);
    mHLine->setPointVisible(false);
    addChild(mVLine);
    addChild(mHLine);
    mStarted = false;
    mLabelNode = new osgEarth::Annotation::PlaceNode();
    mLabelNode->setDynamic(true);
    updateHeightLabel(0);
    mLabelNode->setIconImage(mImageLabel);
    addChild(mLabelNode);
}

void MeasureHeight::setFirstPoint(osgEarth::GeoPoint P1)
{
    mFirstPoint = P1;
    mStarted = true;
}

void MeasureHeight::setSecondPoint(osgEarth::GeoPoint P2)
{
    mSecondPoint = P2;

    if (mSecondPoint.z() > mFirstPoint.z()){
        auto lenght = (mJointPoint.vec3d()-mFirstPoint.vec3d()).length();
        updateHeightLabel(lenght);
    osgEarth::GeoPoint midPoint(mMapItem->getMapSRS(),
                                (mJointPoint.vec3d() + mFirstPoint.vec3d()) / 2);
    mLabelNode->setPosition(midPoint);
    }
    else{
        auto lenght = (mJointPoint.vec3d()-mSecondPoint.vec3d()).length();
        updateHeightLabel(lenght);
        osgEarth::GeoPoint midPoint(mMapItem->getMapSRS(),
                                    (mJointPoint.vec3d() + mSecondPoint.vec3d()) / 2);
        mLabelNode->setPosition(midPoint);
    }

    draw();
}

void MeasureHeight::clear()
{
    mVLine->clear();
    mHLine->clear();
}

void MeasureHeight::draw()
{
    clear();
    if(height()>=0)
    {
        mHLine->addPoint(mFirstPoint);
        mVLine->addPoint(mSecondPoint);
        mJointPoint.set(
                    mMapItem->getMapSRS(), mFirstPoint.x(), mFirstPoint.y(),
                    mSecondPoint.z(), osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    }
    else {
        mHLine->addPoint(mSecondPoint);
        mVLine->addPoint(mFirstPoint);
        mJointPoint.set(
                    mMapItem->getMapSRS(), mSecondPoint.x(), mSecondPoint.y(),
                    mFirstPoint.z(), osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    }
    mHLine->addPoint(mJointPoint);
    mVLine->addPoint(mJointPoint);
}

double MeasureHeight::height()
{
    return  mSecondPoint.z() - mFirstPoint.z();
}

void MeasureHeight::updateHeightLabel(double height)
{
    if (!mRenderImage) {
        mRenderImage = new QImage(
                    LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
                    QImage::Format_RGBA8888
                    );
    }
    if(!mImageLabel.valid())
        mImageLabel = new osg::Image;

    {

        mRenderImage->fill(QColor(Qt::red));
        QPainter painter(mRenderImage);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        static const QBrush backgroundBrush = QBrush(QColor(0, 0, 0, int(255 * 0.3f)));
        static const QFont textFont("SourceSansPro", 10, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));

        painter.setBrush(backgroundBrush);
        painter.drawRoundedRect(
                    mRenderImage->rect(),
                    10,2);

        painter.setPen(textPen);
        painter.setFont(textFont);

        if (height >= 1000){
            height/=1000;
            QString str = QObject::tr("%1 km").arg(height,0,'f',2);
            painter.drawText(0, 0, LABEL_IMAGE_WIDTH, 20,
                             Qt::AlignCenter|Qt::AlignVCenter,
                             str);
        }
        else{
            QString str = QObject::tr("%1 m").arg(height,0,'f',2);
            painter.drawText(0, 0, LABEL_IMAGE_WIDTH, 20,
                             Qt::AlignCenter|Qt::AlignVCenter,
                             str);
        }
    }
    *mRenderImage = mRenderImage->mirrored(false, true);

    mImageLabel->setImage(LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
                    1,
                    GL_RGBA,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    mRenderImage->bits(),
                    osg::Image::AllocationMode::NO_DELETE);
}

float MeasureHeight::getWidth() const
{
    return mWidth;
}

void MeasureHeight::setWidth(float width)
{
    mWidth = width;
    mVLine->setWidth(mWidth);
    mHLine->setWidth(mWidth);
}

osgEarth::Color MeasureHeight::getColor() const
{
    return color;
}

void MeasureHeight::setColor(const osgEarth::Color &value)
{
    color = value;
    mVLine->setFillColor(color);
    mHLine->setFillColor(color);
}

bool MeasureHeight::started() const
{
    return mStarted;
}
