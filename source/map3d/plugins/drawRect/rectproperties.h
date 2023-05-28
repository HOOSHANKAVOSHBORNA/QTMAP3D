#ifndef RECTPROPERTIES_H
#define RECTPROPERTIES_H

#include "rect.h"
#include "mapItem.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>

class RectPropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   fillcolor      READ getFillcolor     WRITE setFillColor    )
    Q_PROPERTY(QString   linecolor      READ getLinecolor     WRITE setLineColor    )
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency )
    Q_PROPERTY(int       lineopacity    READ getLineOpacity   WRITE setLineOpacity  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(double    lineWidth      READ getLineWidth     WRITE setLineWidth    )
    Q_PROPERTY(double    width          READ getWidth         WRITE setWidth        )
    Q_PROPERTY(double    height         READ getHeight        WRITE setHeight       )



public:

    RectPropertiesModel(Rect* rect = nullptr, MapItem *mapItem = nullptr, QObject *parent = nullptr);
    //set fillcolor
    QString getFillcolor() const;
    void setFillColor(const QString &fillcolor);
    //set linecolor
    QString getLinecolor() const;
    void setLineColor(const QString &linecolor);
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
    // set Height
    double getHeight() const;
    void setHeight(double height);
    // set Width
    double getWidth() const;
    void setWidth(double width);

    void setRect(Rect* rect);



signals:

    void rectPropertiesChanged( QVariant );



private:
    QString   mFillcolor ;
    QString   mLinecolor;
    int       mTransparency ;
    int       mLineOpacity  ;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp  = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    double    mLineWidth ;
    double    mWidth;
    double    mHeight;

    Rect*   mRect;
    MapItem* mMapItem{nullptr};



public slots:
};

class RectProperties : public QObject
{
    Q_OBJECT
public:
    RectProperties(Rect *rect, QQmlEngine *engine, UIHandle *uiHandle, QObject *parent = nullptr);
    void show();
    void hide();
    void setRect(Rect* rect);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    RectPropertiesModel* mRectProperties;
    UIHandle* mUiHandle;

};

#endif // RECTPROPERTIES_H
