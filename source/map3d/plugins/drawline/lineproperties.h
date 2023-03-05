#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include "linenode.h"
#include "mapcontroller.h"
#include "plugininterface.h"

#include <QAbstractListModel>
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include <QQuickItem>
#include <QQmlEngine>

class LinePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ getColor         WRITE setColor        )
    Q_PROPERTY(QString   pointColor     READ getPointColor    WRITE setPointColor   )
    Q_PROPERTY(float     width          READ getWidth         WRITE setWidth        )
    Q_PROPERTY(float     pointwidth     READ getPointwidth    WRITE setPointwidth   )
    Q_PROPERTY(float     height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(unsigned  tesselation    READ getTesselation   WRITE setTesselation  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(bool      visible        READ getVisible       WRITE setVisible      )
    Q_PROPERTY(bool      smooth         READ getSmooth        WRITE setSmooth       )


public:

    LinePropertiesModel( QObject *parent = nullptr);
    //set color
    QString getColor() const;
    void setColor(const QString &color);
    //set color
    QString getPointColor() const;
    void setPointColor(const QString &pointColor);
    // set width
    float getWidth() const;
    void setWidth(const float &width);
    // set pointwidth
    float getPointwidth() const;
    void setPointwidth(const float &pointwidth);
    // set height
    float getHeight() const;
    void setHeight(const float &height);
    // set tesselation
    unsigned getTesselation() const;
    void setTesselation(const unsigned &tesselation);
    // set clamp
    int  getClamp() const;
    void setClamp(int clamp);
    //set Type
    bool getVisible() const;
    void setVisible(const bool &visible);
    //set smooth
    bool getSmooth() const;
    void setSmooth(const bool &smooth);

    void setLine(LineNode *linNode) ;





signals:

    void linePropertiesChanged( QVariant );


private:
    QString                                           mColor      ="#91001d" ;
    QString                                           mPointColor ="#001191";
    float                                             mWidth      =10.00;
    float                                             mHeight     ;
    float                                             mPointwidth =10.00;
    unsigned                                          mTesselation=10.00;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp =   osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    bool                                              mVisible    =true;
    bool                                              mSmooth     =true;


    LineNode* mLineNode;




public slots:
};

class LineProperties :public QObject
{
    Q_OBJECT
public:
    LineProperties(QQmlEngine *engine, UIHandle *muiHandle, QObject *parent = nullptr);
    void show();
    void hide();

    void setLine(LineNode *line);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    LinePropertiesModel *mLineProperties;
    UIHandle *mUiHandle = nullptr;

};

#endif // LINEPROPERTIES_H
