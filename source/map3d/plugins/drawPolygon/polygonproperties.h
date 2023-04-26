#ifndef POLYGONPROPERTIES_H
#define POLYGONPROPERTIES_H

#include "polygon.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>

class PolygonPropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QString   linecolor      READ getLinecolor     WRITE setLineColor    )
    Q_PROPERTY(double    height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(int       lineopacity    READ getLineOpacity   WRITE setLineOpacity  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(double    lineWidth      READ getLineWidth     WRITE setLineWidth    )



public:

    PolygonPropertiesModel(QObject *parent = nullptr);
    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString getLinecolor() const;
    void setLineColor(const QString &linecolor);
    //set height
    double getHeight() const;
    void setHeight(const double &polygonheight);
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


    void setPolygon(Polygon* polygon);



signals:

    void polygonPropertiesChanged( QVariant );



private:
    QString   mFillcolor ;
    QString   mLinecolor ;
    double    mHeight   ;
    int       mTransparency ;
    int       mLineOpacity   ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    double    mLineWidth ;

    Polygon*   mPolygon{nullptr};
    MapController* mMapController{nullptr};




public slots:
};

class PolygonProperties : public QObject
{
    Q_OBJECT
public:
    PolygonProperties(QQmlEngine *engine, UIHandle *uiHandle, QObject *parent = nullptr);
    void show();
    void hide();
    void setPolygon(Polygon* polygon);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    PolygonPropertiesModel* mPolygonProperties;
    UIHandle* mUiHandle;

};

#endif // POLYGONPROPERTIES_H