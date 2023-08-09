#ifndef SPHEREPROPERTY_H
#define SPHEREPROPERTY_H

#include "sphereNode.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"




class SphereProperty: public Property
{
    Q_OBJECT
public:
    SphereProperty(QQuickItem *parent=nullptr);
    void setName(const QString& name) override;
    void setFillColor(const QColor &color) override;
    void setRadius(double radius) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setCenter(const QVector3D &center) override;
    SphereNode *getSphere() const;
    void setSphere(SphereNode *Sphere , const osgEarth::SpatialReference *srs);

private:
    SphereNode *mSphere = nullptr;
    const osgEarth::SpatialReference *mSRS;

};
#endif // SPHEREPROPERTY_H
