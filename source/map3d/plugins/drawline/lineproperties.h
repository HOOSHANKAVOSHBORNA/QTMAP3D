#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include "linenode.h"
#include "mapcontroller.h"

#include <QObject>
#include <QVariant>
#include <QVector3D>
#include <QQuickItem>
#include <QQmlEngine>

class LinePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ color         WRITE setColor        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(QString   pointColor     READ pointColor    WRITE setPointColor   NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(float     width          READ width         WRITE setWidth        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(float     pointwidth     READ pointwidth    WRITE setPointwidth   NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(float     height         READ height        WRITE setHeight       NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(unsigned  tesselation    READ tesselation   WRITE setTesselation  NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(osgEarth::Symbology::AltitudeSymbol::Clamping       clamp          READ clamp         WRITE setClamp        NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(bool       visible        READ visible       WRITE setVisible      NOTIFY linePropertiesChangedToQML)
    Q_PROPERTY(bool       smooth         READ smooth        WRITE setSmooth       NOTIFY linePropertiesChangedToQML)


public:

    enum PropertyTypes {
        Color,
        PointColor,
        Width   ,
        Pointwidth ,
        Height,
        Tesselation,
        Clamp      ,
        Visible,
        Smooth
    };
    Q_ENUM(PropertyTypes)




    LinePropertiesModel(LineNode* lineNode = nullptr, QObject *parent = nullptr);
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
    // set height
    float height() const;
    void setHeight(const float &height);
    // set tesselation
    unsigned tesselation() const;
    void setTesselation(const unsigned &tesselation);
    // set clamp
    osgEarth::Symbology::AltitudeSymbol::Clamping  clamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping  &clamp);
    //set Type
    bool visible() const;
    void setVisible(const bool &visible);
    //set smooth
    bool smooth() const;
    void setSmooth(const bool &smooth);





signals:

    void linePropertiesChanged( PropertyTypes  , QVariant );
    void linePropertiesChangedToQML();


private:
    QString        mColor     ;
    QString        mPointColor ;
    float          mWidth      ;
    float          mHeight      ;
    float          mPointwidth ;
    unsigned       mTesselation;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp      ;
    bool            mVisible    ;
    bool            mSmooth;


    LineNode* mLineNode;




public slots:
};

class LineProperties :public QObject
{
    Q_OBJECT
public:
    LineProperties(QQmlEngine *engine, LineNode *line, QObject *parent = nullptr);
    void show();
    void hide();

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    LinePropertiesModel *mLineProperties;
    LineNode *mLine;

};

#endif // LINEPROPERTIES_H
