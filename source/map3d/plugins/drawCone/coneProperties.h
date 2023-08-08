#ifndef CONEPROPERTIESMODEL_H
#define CONEPROPERTIESMODEL_H

#include <QObject>

#include "cone.h"
#include "property.h"

class MainWindow;
class ConeProperties: public Property
{
    Q_OBJECT
public:
    ConeProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(double radius) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setHeight(double height) override;
    void setCenter(const QVector3D &center)override;
    void setName(const QString &name) override;
    Cone *getCone() const;
    void setCone(Cone *cone, const osgEarth::SpatialReference *srs);


private:
    Cone *mCone = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // CONEPROPERTIESMODEL_H

