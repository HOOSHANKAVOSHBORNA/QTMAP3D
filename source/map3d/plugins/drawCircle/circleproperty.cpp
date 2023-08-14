#include "circleproperty.h"
#include <QtDebug>
#include <QVector3D>
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>


#include <QDebug>

CircleProperty::CircleProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setRadiusStatus     (true);
    setArcStatus        (true);
    setStrokeStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setClampStatus      (true);
}

void CircleProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mCircle)
        mCircle->setColor(Utility::qColor2osgEarthColor(color));
}

void CircleProperty::setRadius(double radius)
{
    Property::setRadius(radius);
    if(mCircle)
        mCircle->setRadius(radius);
}
void CircleProperty::setLocationRelative(bool relative)
{
    Property::setLocationRelative(relative);
}

void CircleProperty::setArc(const QVector2D &arc)
{

    Property::setArc(arc);
    if(mCircle){
        mCircle->setArcEnd(arc.y());
        mCircle->setArcStart(arc.x());
        qDebug() << mCircle->getArcEnd().getValue();
    }

}


void CircleProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mCircle)
    {
        mCircle->setName(name.toStdString());

    }
}

void CircleProperty::setStrokeWidth(double opacity)
{

    Property::setStrokeWidth(opacity);
    if(mCircle)
        mCircle->setLineWidth(opacity);

}

void CircleProperty::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mCircle)
    {
        mCircle->setLineColor(Utility::qColor2osgEarthColor(color));
    }

}



void CircleProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCircle)
        mCircle->setPosition(temp);

}

void CircleProperty::setHeight(double height)
{

    Property::setHeight(height);
    if(mCircle)
        mCircle->setCircleHeight(height);
}

void CircleProperty::setClamp(int clamp)
{

    Property::setClamp(clamp);
    if(mCircle){
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
        mCircle->setClamp(clampEnum);

    }
}


Circle *CircleProperty::getCircle() const
{
    return mCircle;
}

void CircleProperty::setCircle(Circle *newCircle, const osgEarth::SpatialReference *srs)
{
    mCircle = newCircle;


    mSRS = srs;

    if(mCircle){
        mCircle->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCircle->setRadius(getRadius());
        mCircle->setCircleHeight(getHeight());
        mCircle->setLineColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mCircle->setLineWidth(getStrokeWidth());
        mCircle->setArcEnd(getArc().y());
        mCircle->setArcStart(getArc().x());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mCircle->setClamp(clampEnum);
        setName(QString::fromStdString(mCircle->getName()));
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCircle->getPosition()));
    }
}





