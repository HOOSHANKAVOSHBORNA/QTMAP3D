#ifndef POLYGONPROPERTIES_H
#define POLYGONPROPERTIES_H

#include "polygon.h"
#include "mapItem.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"






class PolygonProperties: public Property
{
    Q_OBJECT
public:
    PolygonProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setStrokeWidth(const double &opacity);
    void setStroke(const QColor &color)override;
    void setHeight(const double &height) override;
    void setClamp(const int &clamp) override;
    Polygon *getPolygon() const;

    void setPolygon(Polygon *Polygon , const osgEarth::SpatialReference *srs);

private:
    Polygon *mPolygon = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // POLYGONPROPERTIES_H
