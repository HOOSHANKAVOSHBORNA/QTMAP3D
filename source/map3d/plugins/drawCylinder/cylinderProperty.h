#ifndef CylinderPROPERTIESMODEL_H
#define CylinderPROPERTIESMODEL_H

#include <QObject>
#include "cylinder.h"
#include "property.h"



class CylinderProperty: public Property
{
    Q_OBJECT
public:
    CylinderProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(double radius) override;
    void setHeight(double height) override;
    void setLocation();
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setCenter(const QVector3D &center)override;
    void setName(const QString &name) override;
    Cylinder *getCylinder() const;
    void setCylinder(Cylinder *Cylinder, const osgEarth::SpatialReference *srs);

private:
    Cylinder *mCylinder = nullptr;
    const osgEarth::SpatialReference *mSRS;
};




#endif // CylinderPROPERTIESMODEL_H