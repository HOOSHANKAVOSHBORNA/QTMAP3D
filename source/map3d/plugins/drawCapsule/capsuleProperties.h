#ifndef CAPSULEPROPERTIESMODEL_H
#define CAPSULEPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapItem.h"
#include "capsule.h"
#include "property.h"




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
    Capsule *getCapsule() const;
    void setCapsule(Capsule *Capsule , const osgEarth::SpatialReference *srs);

private:
    Capsule *mCapsule = nullptr;
    const osgEarth::SpatialReference *mSRS;

};

#endif // CapsulePROPERTIESMODEL_H


