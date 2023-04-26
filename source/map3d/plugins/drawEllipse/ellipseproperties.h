#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include "ellipse.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>

class EllipsePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QString   linecolor      READ getLinecolor     WRITE setLineColor    )
    Q_PROPERTY(double    ellipseheight  READ getEllipseheight WRITE setEllipseHeight)
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(int       lineopacity    READ getLineOpacity   WRITE setLineOpacity  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(double    lineWidth      READ getLineWidth     WRITE setLineWidth    )



public:

    EllipsePropertiesModel(Ellipse* ellipse = nullptr, MapController *mapController = nullptr, QObject *parent = nullptr);
    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString getLinecolor() const;
    void setLineColor(const QString &linecolor);
    //set height
    double getEllipseheight() const;
    void setEllipseHeight(const double &ellipseheight);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    // set lineopacity
    int getLineOpacity() const;
    void setLineOpacity(const int &lineopacity);
    // set Type
    int getClamp() const;
    void setClamp( int clamp);
    // set lineWidth
    double getLineWidth() const;
    void setLineWidth(double line);

    void setEllipse(Ellipse* ellipse);



signals:

    void ellipsePropertiesChanged( QVariant );



private:
    QString   mFillcolor ;
    QString   mLinecolor;
    double    mEllipseHeight;
    int       mTransparency ;
    int       mLineOpacity  ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    double    mLineWidth ;

    Ellipse*   mEllipse;
    MapController* mMapController{nullptr};



public slots:
};

class EllipseProperties : public QObject
{
    Q_OBJECT
public:
    EllipseProperties(Ellipse *ellipse, QQmlEngine *engine, UIHandle *uiHandle, QObject *parent = nullptr);
    void show();
    void hide();
    void setEllipse(Ellipse* ellipse);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    EllipsePropertiesModel* mEllipseProperties;
    UIHandle* mUiHandle;

};

#endif // CIRCLEPROPERTIES_H
