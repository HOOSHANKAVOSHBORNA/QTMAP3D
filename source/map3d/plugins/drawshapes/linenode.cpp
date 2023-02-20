#include "linenode.h"
#include "drawshapeautoscaler.h"

#include <osg/Point>

LineNode::LineNode(MapController *mapController)
{
    mMapController = mapController;
    mLineGeometry = new osgEarth::Symbology::Geometry();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLineGeometry, mMapController->getMapSRS());
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;

    osgEarth::Symbology::Style pathStyle;
    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mWidth;

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
}

void LineNode::addPoint(osgEarth::GeoPoint point)
{
    mLineGeometry->push_back(point.vec3d());
    dirty();
}

void LineNode::removePoint()
{
    mLineGeometry->pop_back();
    dirty();
}

void LineNode::removeFirstPoint()
{
    mLineGeometry->erase(mLineGeometry->begin());
    dirty();
}

void LineNode::clear()
{
    mLineGeometry->clear();
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
    if(mPointVisible)
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mWidth;
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
    setStyle(style);
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
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mWidth;
    }
    else
        style.remove<osgEarth::Symbology::PointSymbol>();
    setStyle(style);
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
