#ifndef BOXPROPERTIESMODEL_H
#define BOXPROPERTIESMODEL_H

#include <QObject>
#include "box.h"
#include "property.h"
#include <QVariant>
#include <QVector3D>


class BoxProperty: public Property
{
    Q_OBJECT
public:
    BoxProperty(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &position) override;
    void setHeight(double height) override;
    void setCenter(const QVector3D &center)override;
    void setLenght(double lenght) override;
    void setWidth(double width)  override;
    void setName(const QString &name) override;



    Box *getBox() const;
    void setBox(Box *newBox, const osgEarth::SpatialReference *srs);

private:
    Box *mBox = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // BOXPROPERTIESMODEL_H
