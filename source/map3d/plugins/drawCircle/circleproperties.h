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
    void setArc(const QVector2D &arc) override;
    void setStrokeWidth(const double &opacity);
    void setStroke(const QColor &color);
    void setLocation(const QVector3D &status) override;
    void setHeight(const double &height) override;
    void setClamp(const int &clamp) override;
    Circle *getCircle() const;
    void setCircle(Circle* circle);

private:
    Circle *mCircle = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // CIRCLEPROPERTIES_H
