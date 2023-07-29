#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "circle.h"
#include "mapItem.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"


class CircleProperties: public Property
{
    Q_OBJECT
public:
    CircleProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(const double &radius) override;
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &status) override;
    void setArc(const QVector2D &arc) override;
    void setStrokeWidth(const double &opacity)override;
    void setStroke(const QColor &color)override;
    void setHeight(const double &height) override;
    void setClamp(const int &clamp) override;
    void setName(const QString &name) override;
    Circle *getCircle() const;
    void setCircle(Circle* circle, const osgEarth::SpatialReference *srs);



private:
    Circle *mCircle = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // CIRCLEPROPERTIES_H
