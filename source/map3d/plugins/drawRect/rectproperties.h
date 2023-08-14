#ifndef RECTPROPERTIES_H
#define RECTPROPERTIES_H

#include "rect.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"

class RectProperties: public Property
{
    Q_OBJECT
public:
    RectProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setStrokeWidth(double strockWidth)override;
    void setStrokeColor(const QColor &color)override;
    void setHeight(double height) override;
    void setWidth(double width) override;
    void setClamp(int clamp) override;
    void setLocationRelative(bool relative) override;
    void setLocation(const QVector3D &status) override;
    void setName(const QString &name) override;
    Rect *getRect() const;
    void setRect(Rect* Rect, const osgEarth::SpatialReference *srs);



private:
    Rect *mRect = nullptr;
    const osgEarth::SpatialReference *mSRS;
};

#endif // RECTPROPERTIES_H
