#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "circle.h"
#include "mapItem.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include "property.h"

class CirclePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QString   linecolor      READ getLinecolor     WRITE setLineColor    )
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double    radius         READ getRadius        WRITE setRadius       )
    Q_PROPERTY(double    circleheight   READ getCircleheight  WRITE setCircleHeight )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(int       lineopacity    READ getLineOpacity   WRITE setLineOpacity  )
    Q_PROPERTY(double    arcstart       READ getArcstart      WRITE setArcstart     )
    Q_PROPERTY(double    arcend         READ getArcend        WRITE setArcend       )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(bool      relative       READ getRelative      WRITE setRelative     )
    Q_PROPERTY(double    lineWidth      READ getLineWidth     WRITE setLineWidth    )



public:

    CirclePropertiesModel(Circle* circle = nullptr, MapItem *mapController = nullptr, QObject *parent = nullptr);
    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString getLinecolor() const;
    void setLineColor(const QString &linecolor);
    //set location
    QVector3D  getLocation() const;
    void setLocation(const QVector3D  &location);
    // set radius
    double getRadius() const;
    void setRadius(const double &radius);
    //set height
    double getCircleheight() const;
    void setCircleHeight(const double &circleheight);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    // set lineopacity
    int getLineOpacity() const;
    void setLineOpacity(const int &lineopacity);
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
    // set lineWidth
    double getLineWidth() const;
    void setLineWidth(double line);

    void setCircle(Circle* circle);



signals:

    void circlePropertiesChanged( QVariant );
    void positionToQmlChanged();



private:
    QString   mFillcolor ;
    QString   mLinecolor;
    QVector3D  mLocation ;
    double    mRadius   ;
    double    mCircleHeight   ;
    int       mTransparency ;
    int       mLineOpacity  ;
    double    mArcstart  = 0     ;
    double    mArcend    = 360   ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    bool      mRelative  = false ;
    double    mLineWidth ;

    Circle*   mCircle;
    MapItem* mMapItem{nullptr};



public slots:
};
class CircleProperties: public Property
{
    Q_OBJECT
public:
    CircleProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
    void setRadius(const double &radius) override;
    void setLocationRelative(const bool &relative) override;
    void setArc(const QVector2D &arc) override;
    void setStrokeWidth(const double &opacity);
    void setStroke(const QColor &color);
    void setLocation(const QVector3D &status) override;
    void setHeight(const double &height) override;
    void setClamp(const double &clamp) override;
    Circle *getCircle() const;
    void setCircle(Circle* circle);

private:
    Circle *mCircle = nullptr;
};

#endif // CIRCLEPROPERTIES_H
