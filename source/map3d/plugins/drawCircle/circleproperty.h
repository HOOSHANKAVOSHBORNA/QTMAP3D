#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "circle.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"


class CircleProperty: public Property
{
    Q_OBJECT
public:
    CircleProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(double radius) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setArc(const QVector2D &arc) override;
    void setStrokeWidth(double opacity)override;
    void setStrokeColor(const QColor &color)override;
    void setHeight(double height) override;
    void setClamp(int clamp) override;
    void setName(const QString &name) override;
    Circle *getCircle() const;
    void setCircle(Circle* circle, const osgEarth::SpatialReference *srs);



private:
    Circle *mCircle = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // CIRCLEPROPERTIES_H
