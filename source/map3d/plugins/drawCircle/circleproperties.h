#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "circle.h"
#include "mapcontroller.h"
#include "plugininterface.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>

class CirclePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QString   linecolor      READ getLinecolor     WRITE setLineColor    )
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double    radius         READ getRadius        WRITE setRadius       )
    Q_PROPERTY(double    circleheight   READ getCircleheight  WRITE setCircleHeight )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(double    arcstart       READ getArcstart      WRITE setArcstart     )
    Q_PROPERTY(double    arcend         READ getArcend        WRITE setArcend       )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(bool      relative       READ getRelative      WRITE setRelative     )



public:

    CirclePropertiesModel(MapController *mapController = nullptr, QObject *parent = nullptr);
    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString getLinecolor() const;
    void setLineColor(const QString &linecolor);
    //set location
    QVector3D getLocation() const;
    void setLocation(const QVector3D &location);
    // set radius
    double getRadius() const;
    void setRadius(const double &radius);
    //set height
    double getCircleheight() const;
    void setCircleHeight(const double &circleheight);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    //set Arc start
    double getArcstart() const;
    void setArcstart(const double &arcstart);
    //set Arc end
    double getArcend() const;
    void setArcend(const double &arcend);
    // set Type
    int getClamp() const;
    void setClamp( int clamp);
    // set relative
    bool getRelative() const;
    void setRelative(const bool &relative);
    void setCircle(Circle* circle);



signals:

    void circlePropertiesChanged( QVariant );
    void positionToQmlChanged();



private:
    QString   mFillcolor = "#91001d";
    QString   mLinecolor = "#001191";
    QVector3D mLocation ;
    double    mRadius  = 200000  ;
    double    mCircleHeight = 0  ;
    int       mTransparency = 50 ;
    double    mArcstart  = 0     ;
    double    mArcend    = 360   ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    bool      mRelative  = false ;

    Circle*   mCircle;
    MapController* mMapController{nullptr};



public slots:
};

class CircleProperties : public QObject
{
    Q_OBJECT
public:
    CircleProperties(QQmlEngine *engine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent = nullptr);
    void show();
    void hide();
    void setCircle(Circle* circle);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    CirclePropertiesModel* mCircleProperties;
    UIHandle* mUiHandle;

};

#endif // CIRCLEPROPERTIES_H
