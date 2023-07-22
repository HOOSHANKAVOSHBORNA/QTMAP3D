#ifndef BOXPROPERTIESMODEL_H
#define BOXPROPERTIESMODEL_H

#include <QObject>

#include "box.h"
#include "property.h"



class BoxProperty: public Property
{
    Q_OBJECT
public:
    BoxProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &position) override;
    void setHeight(const double &height) override;
    void setLenght(const double &lenght) override;
    void setWidth( const double &width)  override;
    void setClamp(const int &clamp) override;
//    void setCenter(const QVector3D &center) override;


    Box *getBox() const;
    void setBox(Box *newBox, const osgEarth::SpatialReference *srs);

private:
    Box *mBox = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // BOXPROPERTIESMODEL_H
