#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "circle.h"
#include "mapcontroller.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>

class CircleProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ fillcolor     WRITE setFillColor    NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(QString   linecolor      READ linecolor     WRITE setLineColor    NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(QVector3D location       READ location      WRITE setLocation     NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(double    radius         READ radius        WRITE setRadius       NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(double    circleheight   READ circleheight  WRITE setCircleHeight NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(int       transparency   READ transparency  WRITE setTransparency NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(double    arcstart       READ arcstart      WRITE setArcstart     NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(double    arcend         READ arcend        WRITE setArcend       NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(osgEarth::Symbology::AltitudeSymbol::Clamping       clamp          READ clamp         WRITE setClamp        NOTIFY circlePropertiesChangedToQML)
    Q_PROPERTY(bool      relative       READ relative      WRITE setRelative     NOTIFY circlePropertiesChangedToQML)


public:

    enum PropertyTypes {
        FillColor,
        LineColor,
        Location,
        Radius,
        CircleHeight,
        Transparency,
        ArcStart,
        ArcEnd,
        Clamp,
        Relative
    };
    Q_ENUM(PropertyTypes)



    CircleProperties(Circle* Circle = nullptr, MapController *mapController = nullptr, QObject *parent = nullptr);
    //set fillcolor
    QString fillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString linecolor() const;
    void setLineColor(const QString &linecolor);
    //set location
    QVector3D location() const;
    void setLocation(const QVector3D &location);
    // set radius
    double radius() const;
    void setRadius(const double &radius);
    //set height
    double circleheight() const;
    void setCircleHeight(const double &circleheight);
    // set transparency
    int transparency() const;
    void setTransparency(const int &transparency);
    //set Arc start
    double arcstart() const;
    void setArcstart(const double &arcstart);
    //set Arc end
    double arcend() const;
    void setArcend(const double &arcend);
    // set Type
    osgEarth::Symbology::AltitudeSymbol::Clamping clamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp);
    // set relative
    bool relative() const;
    void setRelative(const bool &relative);



signals:

    void circlePropertiesChanged( PropertyTypes  , QVariant );
    void circlePropertiesChangedToQML();


private:
    QString   mFillcolor;
    QString   mLinecolor;
    QVector3D mLocation ;
    double    mRadius ;
    double    mCircleHeight ;
    int       mTransparency;
    double    mArcstart ;
    double    mArcend ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  ;
    bool      mRelative ;

    Circle*   mCircle;
    MapController* mMapController{nullptr};



public slots:
};

#endif // CIRCLEPROPERTIES_H
