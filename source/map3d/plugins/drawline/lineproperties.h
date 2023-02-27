#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include "linenode.h"
#include "mapcontroller.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>

class LineProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ color         WRITE setColor        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(QString   pointColor     READ pointColor    WRITE setPointColor   NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(float     width          READ width         WRITE setWidth        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(float     pointwidth     READ pointwidth    WRITE setPointwidth   NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(unsigned  tesselation    READ tesselation   WRITE setTesselation  NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(osgEarth::Symbology::AltitudeSymbol::Clamping       clamp          READ clamp         WRITE setClamp        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(int       visible        READ visible       WRITE setVisible      NOTIFY linePropertiesChangedToQML)


public:

    enum PropertyTypes {
        Color,
        PointColor,
        Width   ,
        Pointwidth ,
        Tesselation,
        Clamp      ,
        Visible
    };
    Q_ENUM(PropertyTypes)




    LineProperties(LineNode* lineNode = nullptr/*, MapController *mapController = nullptr*/, QObject *parent = nullptr);
    //set color
    QString color() const;
    void setColor(const QString &color);
    //set color
    QString pointColor() const;
    void setPointColor(const QString &pointColor);
    // set width
    float width() const;
    void setWidth(const float &width);
    // set pointwidth
    float pointwidth() const;
    void setPointwidth(const float &pointwidth);
    // set tesselation
    unsigned tesselation() const;
    void setTesselation(const unsigned &tesselation);
    // set clamp
    osgEarth::Symbology::AltitudeSymbol::Clamping  clamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping  &clamp);
    //set Type
    int visible() const;
    void setVisible(const int &visible);





signals:

    void linePropertiesChanged( PropertyTypes  , QVariant );
    void linePropertiesChangedToQML();


private:
    QString        mColor     ;
    QString        mPointColor ;
    float          mWidth      ;
    float          mPointwidth ;
    unsigned       mTesselation;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp      ;
    int            mVisible    ;


    LineNode* mLineNode;
    MapController* mMapController{nullptr};


public slots:
};

#endif // LINEPROPERTIES_H
