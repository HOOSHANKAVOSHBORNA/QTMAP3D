#ifndef POLYGONPROPERTIES_H
#define POLYGONPROPERTIES_H

#include "polygon.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"

class PolygonProperty: public Property
{
    Q_OBJECT
public:
    PolygonProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setStrokeWidth(const double &opacity);
    void setStrokeColor(const QColor &color)override;
    void setHeight(double height) override;
    void setClamp(int clamp) override;
    void setName(const QString &name) override;
    Polygon *getPolygon() const;

    void setPolygon(Polygon *Polygon , const osgEarth::SpatialReference *srs);

private:
    Polygon *mPolygon = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // POLYGONPROPERTIES_H
