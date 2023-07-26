#ifndef CylinderPROPERTIESMODEL_H
#define CylinderPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapItem.h"
#include "cylinder.h"
#include "property.h"



class CylinderProperties: public Property
{
    Q_OBJECT
public:
    CylinderProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(const double &radius) override;
    void setHeight(const double &height) override;
    void setLocation();
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &status) override;
    Cylinder *getCylinder() const;
    void setCylinder(Cylinder *Cylinder, const osgEarth::SpatialReference *srs);

private:
    Cylinder *mCylinder = nullptr;
    const osgEarth::SpatialReference *mSRS;
};




#endif // CylinderPROPERTIESMODEL_H
