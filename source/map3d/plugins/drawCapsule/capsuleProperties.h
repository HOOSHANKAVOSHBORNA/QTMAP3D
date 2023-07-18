#ifndef CAPSULEPROPERTIESMODEL_H
#define CAPSULEPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapItem.h"
#include "capsule.h"
#include "property.h"

class CapsulePropertiesModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double    radius         READ getRadius        WRITE setRadius       )
    Q_PROPERTY(double    height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )

public:
    CapsulePropertiesModel(Capsule *Capsule = nullptr, MapItem *mapcontroller = nullptr, QObject *parent = nullptr);

    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set location
    QVector3D  getLocation() const;
    void setLocation(const QVector3D  &location);
    // set radius
    double getRadius() const;
    void setRadius(const double &radius);
    //set height
    double getHeight() const;
    void setHeight(const double &CapsuleHeight);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    // set relative
    bool getRelative() const;
    void setRelative(const bool &value);

    void setCapsule(Capsule *Capsule);

signals:

    void circlePropertiesChanged( QVariant );
    void positionToQmlChanged();

private:
    QString   mFillcolor ;
    QVector3D  mLocation ;
    double    mRadius  = 0;
    double    mHeight = 0;
    int       mTransparency = 0;
    bool      mRelative = false;

    Capsule *mCapsule;
    MapItem *mMapItem {nullptr};

};

//class CapsuleProperties: public QObject
//{
//    Q_OBJECT

//public:
//    CapsuleProperties(Capsule *Capsule, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapcontroller, QObject *parent = nullptr);

//    void show();
//    void hide();
//    void setCapsule(Capsule *Capsule);
//    void setLocation(osgEarth::GeoPoint location);

//private:
//    QQmlEngine* mQmlEngine;
//    QQuickItem* mItem;
//    CapsulePropertiesModel* mCapsuleProperties;
//    UIHandle* mUiHandle;
//};


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

private:
    Capsule *mCapsule = nullptr;
};

#endif // CapsulePROPERTIESMODEL_H


