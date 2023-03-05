#include "linenode.h"
#include "drawshapeautoscaler.h"
#include <osgEarth/GeoMath>
#include <osg/Point>

LineNode::LineNode(MapController *mapController)
{
    mMapController = mapController;
    mLineGeometry = new osgEarth::Symbology::Geometry();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLineGeometry, mMapController->getMapSRS());
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;

    osgEarth::Symbology::Style pathStyle;
    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;

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
    mLableGroup = new osg::Group;
}

void LineNode::addPoint(osgEarth::GeoPoint point)
{
    mLineGeometry->push_back(point.vec3d());
//    qDebug()<< getSize();
    dirty();
    if(getSize() >= 2)
    {
        std::vector<osg::Vec3d> distanceVectorPoint;
        distanceVectorPoint.push_back(mLineGeometry->at(mLineGeometry->size() - 2));
        distanceVectorPoint.push_back(mLineGeometry->at(mLineGeometry->size() - 1));

        auto lenght = osgEarth::GeoMath().rhumbDistance(distanceVectorPoint);

        auto imageLabel = updateLenghtLable(lenght);
        osg::ref_ptr<osgEarth::Annotation::PlaceNode> labelNode = new osgEarth::Annotation::PlaceNode();
        labelNode->setIconImage(imageLabel);
        osgEarth::GeoPoint midPoint(mMapController->getMapSRS(),
                                    (mLineGeometry->at(mLineGeometry->size() - 2) + mLineGeometry->at(mLineGeometry->size() -1 )) / 2);
        labelNode->setPosition(midPoint);
        mLableGroup->addChild(labelNode);
        addChild(mLableGroup);
    }
}

void LineNode::removePoint()
{
    mLineGeometry->pop_back();
    dirty();
    mLableGroup->removeChild(getSize()-1);
}

void LineNode::removeFirstPoint()
{
    mLineGeometry->erase(mLineGeometry->begin());
    dirty();
    mLableGroup->removeChild(0, 1);
}

void LineNode::clear()
{
    mLineGeometry->clear();
    mLableGroup->removeChildren(0,mLableGroup->getNumChildren());
}

int LineNode::getSize()
{
    return static_cast<int>(mLineGeometry->size());
}

void LineNode::showLenght(bool show)
{
    mLableGroup->setNodeMask(show);
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
    }
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
    addChild(mLableGroup);
}

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
    }
    else
        style.remove<osgEarth::Symbology::PointSymbol>();
    setStyle(style);
    addChild(mLableGroup);
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
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
    setStyle(style);
}

osg::Image* LineNode::updateLenghtLable(double lenght)
{
    if (!mRenderImage) {
        mRenderImage = new QImage(
                    LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
                    QImage::Format_RGBA8888
                    );
    }
    osg::Image* image = new osg::Image;
    {
        mRenderImage->fill(QColor(Qt::red));
        QPainter painter(mRenderImage);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        static const QBrush backgroundBrush = QBrush(QColor(0, 0, 0, int(255 * 0.3f)));
        static const QFont textFont("SourceSansPro", 12, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));

        painter.setBrush(backgroundBrush);
        painter.drawRoundedRect(
                    mRenderImage->rect(),
                    10,2);

        painter.setPen(textPen);
        painter.setFont(textFont);
        if (lenght >= 1000){
            lenght/=1000;
            QString str = QObject::tr("%1 km").arg(lenght,0,'f',2);
            painter.drawText(mRenderImage->rect(),
                             Qt::AlignCenter,
                             str);
        }
        else{
            QString str = QObject::tr("%1 m").arg(lenght);
            painter.drawText(mRenderImage->rect(),
                             Qt::AlignCenter,
                             str);
        }

    }
    *mRenderImage = mRenderImage->mirrored(false, true);

    image->setImage(LABEL_IMAGE_WIDTH,
                          LABEL_IMAGE_HEIGHT,
                          1,
                          GL_RGBA,
                          GL_RGBA,
                          GL_UNSIGNED_BYTE,
                          mRenderImage->bits(),
                          osg::Image::AllocationMode::NO_DELETE);
    return image;
}
bool LineNode::getSmooth() const
{
    return mSmooth;
}

void LineNode::setSmooth(bool smooth)
{
    mSmooth = smooth;
    auto sStyle = getStyle();
    if(mSmooth)
        sStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;
    setStyle(sStyle);
    addChild(mLableGroup);
}
