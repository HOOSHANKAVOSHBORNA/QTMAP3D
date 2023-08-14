#include "polygonProperty.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "property.h"
#include "utility.h"

PolygonProperty::PolygonProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setStrokeStatus     (true);
    setHeightStatus     (true);
    setClampStatus      (true);
}

void PolygonProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mPolygon)
        mPolygon->setFillColor(Utility::qColor2osgEarthColor(color));
}


void PolygonProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mPolygon)
        mPolygon->setName(name.toStdString());

}

void PolygonProperty::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mPolygon)
        mPolygon->setStrokeWidth(opacity);

}

void PolygonProperty::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mPolygon)
    {
        mPolygon->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }

}

void PolygonProperty::setHeight(double height)
{

    Property::setHeight(height);
    if(mPolygon)
        mPolygon->setHeight(height);
}

void PolygonProperty::setClamp(int clamp)
{

    Property::setClamp(clamp);
    if(mPolygon){
//        qDebug()<<"clamp:"<<clamp;
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
//        qDebug()<<"clampEnum:"<<clampEnum;
        mPolygon->setClamp(clampEnum);

    }
}


Polygon *PolygonProperty::getPolygon() const
{
    return mPolygon;
}

void PolygonProperty::setPolygon(Polygon *newPolygon, const osgEarth::SpatialReference *srs)
{
    mPolygon = newPolygon;
    mSRS = srs;

    if(mPolygon){
        mPolygon->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mPolygon->setHeight(getHeight());
        mPolygon->setStrokeColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mPolygon->setStrokeWidth(getStrokeWidth());
        setName(QString::fromStdString(mPolygon->getName()));
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mPolygon->setClamp(clampEnum);
    }
}
