#ifndef SPHEREPROPERTIES_H
#define SPHEREPROPERTIES_H

#include "spherenode.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>

class SphereProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ color         WRITE setColor        )
    Q_PROPERTY(QVector3D location       READ location      WRITE setLocation     )
    Q_PROPERTY(QVector3D center         READ center        WRITE setCenter       )
    Q_PROPERTY(double    radius         READ radius        WRITE setRadius       )
    Q_PROPERTY(QString   transparency   READ transparency  WRITE setTransparency )
    Q_PROPERTY(int       shape          READ shape         WRITE setShape        )
    Q_PROPERTY(bool      relative       READ relative      WRITE setRelative     )


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



    SphereProperties(SphereNode* sphereNode = nullptr, QObject *parent = nullptr);
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
    QString transparency() const;
    void setTransparency(const QString &transparency);
    // set Type
    int shape() const;
    void setShape(const int &type);
    // set relative
    bool relative() const;
    void setRelative(const bool &relative);



signals:

    void spherePropertiesChanged( PropertyTypes  , QVariant );
 //   void sphereLocationChanged  ( PropertyTypes  , QVector3D);


private:
    QString   mColor;
    QVector3D mLocation;
    QVector3D mCenter;
    double    mRadius;
    QString   mTransparency;
    int       mShape;
    bool      mRelative;

    SphereNode* mSphereNode;


public slots:

};

#endif // SPHEREPROPERTIES_H
