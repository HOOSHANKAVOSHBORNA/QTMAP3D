#ifndef SPHEREPROPERTIES_H
#define SPHEREPROPERTIES_H

#include "sphereNode.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"




class SphereProperties: public Property
{
    Q_OBJECT
public:
    SphereProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(double radius) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
//    void setCenter();
    SphereNode *getSphere() const;
    void setSphere(SphereNode *Sphere , const osgEarth::SpatialReference *srs);

private:
    SphereNode *mSphere = nullptr;
    const osgEarth::SpatialReference *mSRS;

};
#endif // SPHEREPROPERTIES_H
