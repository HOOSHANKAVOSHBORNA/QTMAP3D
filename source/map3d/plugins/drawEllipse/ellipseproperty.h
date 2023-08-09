#ifndef ELLIPSEPROPERTY_H
#define ELLIPSEPROPERTY_H

#include "ellipse.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"


class EllipseProperty: public Property
{
    Q_OBJECT
public:
    EllipseProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setStrokeWidth(const double &opacity);
    void setStrokeColor(const QColor &color)override;
    void setHeight(double height) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setClamp(int clamp) override;
    void setName(const QString &name) override;


    Ellipse *getEllipse() const;
    void setEllipse(Ellipse *Ellipse , const osgEarth::SpatialReference *srs);

private:
    Ellipse *mEllipse = nullptr;
     const osgEarth::SpatialReference *mSRS;
};

#endif // ELLIPSEPROPERTY_H
