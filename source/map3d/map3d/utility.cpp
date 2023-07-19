#include "utility.h"
#include <osgEarth/GeoData>

Utility::Utility()
{

}

QColor Utility::osgEarthColorToQColor(const osgEarth::Color &color)
{
    return QColor(color.r()*255, color.g()*255, color.b()*255, color.a()*255);
}

osgEarth::Color Utility::qColor2osgEarthColor(const QColor &color){
    return osgEarth::Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

QVector3D Utility::osgEarthGeoPointToQvector3D(const osgEarth::GeoPoint &position)
{
    QVector3D temp;
    temp.setX(position.x());
    temp.setY(position.y());
    temp.setZ(position.z());
    return temp;

}

osgEarth::GeoPoint Utility::qVector3DToosgEarthGeoPoint(const QVector3D &position, const osgEarth::SpatialReference* srs)
{
    osgEarth::GeoPoint tempLocation(srs, position.x(), position.y(), position.z());
//    tempLocation.x() = static_cast<double>(position.x());
//    tempLocation.y() = static_cast<double>(position.y());
//    tempLocation.z() = static_cast<double>(position.z());
    return tempLocation;
}
