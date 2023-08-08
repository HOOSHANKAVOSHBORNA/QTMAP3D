#include "lineProperty.h"
#include <QtDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <utility.h>
#include "property.h"


LineProperty::LineProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus(false);
    setStrokeStatus(true);
    setPointsStatus(true);
    setBearingStatus(true);
    setShowSlopStatus(true);
    setTesselationStatus(true);
    setClampStatus(true);
    setShowLenStatus(true);
}


void LineProperty::setStrokeWidth(double width)
{

    Property::setStrokeWidth(width);
    if(mLineNode)
        mLineNode->setWidth(width);
}

void LineProperty::setStrokeColor(const QColor &color)
{
    Property::setStrokeColor(color);
    if(mLineNode)
        mLineNode->setColor(Utility::qColor2osgEarthColor(color));

}

void LineProperty::setPointsColor(const QColor &point)
{
    Property::setPointsColor(point);
    if(mLineNode)
        mLineNode->setPointColor(Utility::qColor2osgEarthColor(point));

}

void LineProperty::setPointsWidth(double point)
{
    Property::setPointsWidth(point);
    if(mLineNode)
        mLineNode->setPointWidth(point);
}

void LineProperty::setPointsSmooth(bool point)
{
    Property::setPointsSmooth(point);
    if(mLineNode)
        mLineNode->setSmooth(point);
}

void LineProperty::setShowBearing(bool showBearing)
{
    Property::setShowBearing(showBearing);
    if(mLineNode)
        mLineNode->setShowBearing(showBearing);
}

void LineProperty::setShowSlop(bool slop)
{
    Property::setShowSlop(slop);
    if(mLineNode)
        mLineNode->setShowSlope(slop);
}

void LineProperty::setTesselation(double tesselation)
{
    Property::setTesselation(tesselation);
    if(mLineNode)
        mLineNode->setTessellation(tesselation);
}

void LineProperty::setClamp(int clamp)
{
    Property::setClamp(clamp);
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
    mLineNode->setClamp(clampEnum);
}

void LineProperty::setShowLen(bool showLen)
{
    Property::setShowLen(showLen);
    if(mLineNode)
        mLineNode->setShowDistance(showLen);
}


LineNode *LineProperty::getLine() const
{
    return mLineNode;
}

void LineProperty::setLine(LineNode *newLine)
{
    mLineNode = newLine;
    if(mLineNode){
        mLineNode->setWidth(getStrokeWidth());
        mLineNode->setColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mLineNode->setPointColor(Utility::qColor2osgEarthColor(getPointsColor()));
        mLineNode->setPointWidth(getPointsWidth());
        mLineNode->setSmooth(getPointsSmooth());
        mLineNode->setShowBearing(getShowBearing());
        mLineNode->setShowSlope(getShowSlop());
        mLineNode->setTessellation(getTesselation());

        mLineNode->setShowDistance(getShowLen());
    }
}
