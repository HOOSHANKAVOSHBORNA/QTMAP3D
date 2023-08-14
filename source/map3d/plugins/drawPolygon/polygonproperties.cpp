#include "polygonproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "property.h"
#include "utility.h"

PolygonProperties::PolygonProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setStrokeStatus     (true);
    setHeightStatus     (true);
    setClampStatus      (true);
    setPointsStatus     (true);
}

void PolygonProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mPolygon)
        mPolygon->setFillColor(Utility::qColor2osgEarthColor(color));
}


void PolygonProperties::setName(const QString &name)
{

    Property::setName(name);
    if(mPolygon)
    {
        mPolygon->setName(name.toStdString());

    }
}

void PolygonProperties::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mPolygon)
        mPolygon->setLineWidth(opacity);

}

void PolygonProperties::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mPolygon)
    {
        mPolygon->setLineColor(Utility::qColor2osgEarthColor(color));
    }

}

void PolygonProperties::setHeight(double height)
{

    Property::setHeight(height);
    if(mPolygon)
        mPolygon->setHeight(height);
}

void PolygonProperties::setClamp(int clamp)
{

    Property::setClamp(clamp);
    if(mPolygon){
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
        mPolygon->setClamp(clampEnum);

    }
}


Polygon *PolygonProperties::getPolygon() const
{
    return mPolygon;
}

void PolygonProperties::setPolygon(Polygon *newPolygon, const osgEarth::SpatialReference *srs)
{
    mPolygon = newPolygon;


    mSRS = srs;

    if(mPolygon){
        mPolygon->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mPolygon->setHeight(getHeight());
        mPolygon->setLineColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mPolygon->setLineWidth(getStrokeWidth());
        setName(QString::fromStdString(mPolygon->getName()));
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mPolygon->setClamp(clampEnum);

    }
}
