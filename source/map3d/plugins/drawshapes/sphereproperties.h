#ifndef SPHEREPROPERTIES_H
#define SPHEREPROPERTIES_H

#include "spherenode.h"
#include "mapcontroller.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>

class SphereProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ color         WRITE setColor        NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(QVector3D location       READ location      WRITE setLocation     NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(QVector3D center         READ center        WRITE setCenter       NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(double    radius         READ radius        WRITE setRadius       NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(int       transparency   READ transparency  WRITE setTransparency NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(int       shape          READ shape         WRITE setShape        NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(bool      relative       READ relative      WRITE setRelative     NOTIFY spherePropertiesChangedToQML)


public:

    enum PropertyTypes {
        Color,
        Location,
        Center,
        Radius,
        Transparency,
        Shape,
        Relative
    };
    Q_ENUM(PropertyTypes)




    SphereProperties(SphereNode* sphereNode = nullptr, MapController *mapController = nullptr, QObject *parent = nullptr);
    //set color
    QString color() const;
    void setColor(const QString &color);
    //set location
    QVector3D location() const;
    void setLocation(const QVector3D &location);
    //set center
    QVector3D center() const;
    void setCenter(const QVector3D &center);
    // set radius
    double radius() const;
    void setRadius(const double &radius);
    // set transparency
    int transparency() const;
    void setTransparency(const int &transparency);
    // set Type
    int shape() const;
    void setShape(const int &type);
    // set relative
    bool relative() const;
    void setRelative(const bool &relative);




signals:

    void spherePropertiesChanged( PropertyTypes  , QVariant );
    void spherePropertiesChangedToQML();


private:
    QString     mSphereColor;
    QVector3D   mLocation;
    QVector3D   mCenter;
    double      mRadius;
    int         mTransparency;
    int         mShape;
    bool        mRelative;


    SphereNode* mSphereNode;
    MapController* mMapController{nullptr};


public slots:
};

#endif // SPHEREPROPERTIES_H
