#ifndef CONEPROPERTIESMODEL_H
#define CONEPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapcontroller.h"
#include "cone.h"

class ConePropertiesModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double    radius         READ getRadius        WRITE setRadius       )
    Q_PROPERTY(double    height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )

public:
    ConePropertiesModel(Cone *cone = nullptr, MapController *mapcontroller = nullptr, QObject *parent = nullptr);

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
    void setHeight(const double &coneHeight);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    // set Type
    int getClamp() const;
    void setClamp( int clamp);

    void setCone(Cone *cone);

signals:

    void circlePropertiesChanged( QVariant );
    void positionToQmlChanged();

private:
    QString   mFillcolor ;
    QVector3D  mLocation ;
    double    mRadius  = 20000  ;
    double    mHeight = 0  ;
    int       mTransparency ;
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;

    Cone *mCone;
    MapController *mMapController {nullptr};

};

class ConeProperties: public QObject
{
    Q_OBJECT

public:
    ConeProperties(Cone *cone, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent = nullptr);

    void show();
    void hide();
    void setCone(Cone *cone);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    ConePropertiesModel* mConeProperties;
    UIHandle* mUiHandle;
};

#endif // CONEPROPERTIESMODEL_H
