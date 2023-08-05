#ifndef CAPSULEPROPERTIESMODEL_H
#define CAPSULEPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapItem.h"
#include "capsule.h"
#include "property.h"
#include <QVariant>
#include <QVector3D>



class CapsuleProperties: public Property
{
    Q_OBJECT
public:
    CapsuleProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(const double &radius) override;
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &status) override;
    void setHeight(const double &height) override;
    void setCenter(const QVector3D &center)override;
    void setName(const QString &name) override;
    Capsule *getCapsule() const;
    void setCapsule(Capsule *Capsule , const osgEarth::SpatialReference *srs);

private:
    Capsule *mCapsule = nullptr;
    const osgEarth::SpatialReference *mSRS;

};

#endif // CapsulePROPERTIESMODEL_H


