#include "rectproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "property.h"
#include "utility.h"

RectProperties::RectProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setStrokeStatus     (true);
    setClampStatus      (true);
    setHeightStatus     (true);
    setWidthStatus      (true);
    setLocationStatus   (true);
}

void RectProperties::setFillColor(const QColor &color)
{
    Property::setFillColor(color);
    if(mRect)
        mRect->setFillColor(Utility::qColor2osgEarthColor(color));
}

void RectProperties::setHeight(double height)
{
    Property::setHeight(height);
    if(mRect)
        mRect->setHeight(height);
}

void RectProperties::setWidth(double width)
{
    Property::setWidth(width);
    if(mRect)
    mRect->setWidth(width);
}

void RectProperties::setStrokeWidth(double strockWidth)
{
    Property::setStrokeWidth(strockWidth);
    if(mRect)
        mRect->setStrokeWidth(strockWidth);
}

void RectProperties::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mRect)
    {
        mRect->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }
}

void RectProperties::setLocationRelative(bool relative)
{
    Property::setLocationRelative(relative);
}



void RectProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mRect)
        mRect->setPosition(temp);
}

void RectProperties::setName(const QString &name)
{
    Property::setName(name);
    if(mRect)
    {
        mRect->setName(name.toStdString());
    }
}

void RectProperties::setClamp(int clamp)
{
    Property::setClamp(clamp);
    if(mRect){
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
        mRect->setClamp(clampEnum);
    }
}


Rect *RectProperties::getRect() const
{
    return mRect;
}

void RectProperties::setRect(Rect *newRect, const osgEarth::SpatialReference *srs)
{
    mRect = newRect;


    mSRS = srs;

    if(mRect){
        mRect->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mRect->setStrokeColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mRect->setStrokeWidth(getStrokeWidth());
        mRect->setHeight(getHeight());
        mRect->setWidth(getWidth());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mRect->setClamp(clampEnum);
        setLocation(Utility::osgEarthGeoPointToQvector3D(mRect->getPosition()));
        setName(QString::fromStdString(mRect->getName()));
    }
}
