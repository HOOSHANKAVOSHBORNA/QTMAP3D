#include "rectproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "property.h"
#include "mainwindow.h"
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
        mRect->setColor(Utility::qColor2osgEarthColor(color));
}


void RectProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if(mRect)
//        mRect->setHeight(osgEarth::Distance(height, osgEarth::Units::METERS));
        mRect->setHeight(height);
}

void RectProperties::setWidth(const double &width)
{
    Property::setWidth(width);
    if(mRect)
//        mRect->setWidth(osgEarth::Distance(width, osgEarth::Units::METERS));
    mRect->setWidth(width);
}


void RectProperties::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mRect)
        mRect->setStrokeWidth(opacity);
}

void RectProperties::setStroke(const QColor &color)
{

    Property::setStroke(color);
    if(mRect)
    {
        mRect->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }

}


void RectProperties::setLocationRelative(const bool &relative)
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




void RectProperties::setClamp(const int &clamp)
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
        mRect->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mRect->setStrokeColor(Utility::qColor2osgEarthColor(getStroke()));
        mRect->setStrokeWidth(getStrokeWidth());

//        mRect->setHeight(osgEarth::Distance(mRect->getHeight().getValue(), osgEarth::Units::METERS));
        mRect->setHeight(getHeight());
//        mRect->setWidth(osgEarth::Distance(mRect->getWidth().getValue(), osgEarth::Units::METERS));
        mRect->setWidth(getWidth());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mRect->setClamp(clampEnum);
        setLocation(Utility::osgEarthGeoPointToQvector3D(mRect->getPosition()));
        setName(QString::fromStdString(mRect->getName()));
    }
}
