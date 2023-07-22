#ifndef UTILITY_H
#define UTILITY_H

#include <QColor>
#include <QVector3D>
#include <osgEarth/Color>

namespace osgEarth {
class GeoPoint;
class SpatialReference;
}

class Utility
{
public:
    Utility();
    static QColor osgEarthColorToQColor(const osgEarth::Color &color);
    static osgEarth::Color qColor2osgEarthColor(const QColor &color);
    static osgEarth::GeoPoint qVector3DToosgEarthGeoPoint(const QVector3D &position, const osgEarth::SpatialReference *srs);
    static QVector3D osgEarthGeoPointToQvector3D(const osgEarth::GeoPoint &position);
};

#endif // UTILITY_H
