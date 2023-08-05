#ifndef ELLIPSEPROPERTIES_H
#define ELLIPSEPROPERTIES_H

#include "ellipse.h"
#include "mapItem.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"


class EllipseProperties: public Property
{
    Q_OBJECT
public:
    EllipseProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setStrokeWidth(const double &opacity);
    void setStroke(const QColor &color)override;
    void setHeight(const double &height) override;
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &status) override;
    void setClamp(const int &clamp) override;
    void setName(const QString &name) override;


    Ellipse *getEllipse() const;
    void setEllipse(Ellipse *Ellipse , const osgEarth::SpatialReference *srs);

private:
    Ellipse *mEllipse = nullptr;
     const osgEarth::SpatialReference *mSRS;
};

#endif // ELLIPSEPROPERTIES_H
