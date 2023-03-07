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
    Q_PROPERTY(QString   width          READ getWidth         WRITE setWidth        )
    Q_PROPERTY(float     pointwidth     READ getPointwidth    WRITE setPointwidth   )
    Q_PROPERTY(float     height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(unsigned  tesselation    READ getTesselation   WRITE setTesselation  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(bool      visible        READ getVisible       WRITE setVisible      )
    Q_PROPERTY(bool      smooth         READ getSmooth        WRITE setSmooth       )
    Q_PROPERTY(bool      showLen        READ getShowLen       WRITE setShowLen      )
    Q_PROPERTY(bool      ruler          READ getRuler         WRITE setRuler        NOTIFY rulerChanged )






public:

    LinePropertiesModel( QObject *parent = nullptr);
    //set color
    QString getColor() const;
    void setColor(const QString &color);
    //set color
    QString getPointColor() const;
    void setPointColor(const QString &pointColor);
    // set width
    int getWidth() const;
    void setWidth(const QString &width);
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
    //set smooth
    bool getShowLen() const;
    void setShowLen(const bool &smooth);

    bool getRuler() const;
    void setRuler(const bool ruler);

    void setLine(LineNode *linNode) ;
signals:

    void linePropertiesChanged(  QVariant );
    void rulerChanged();


private:
    QString                                           mColor       = "#91001d";
    QString                                           mPointColor  = "#001191";
    int                                               mWidth       = 10.00;
    float                                             mHeight;
    float                                             mPointwidth  = 10.00;
    unsigned                                          mTesselation =10.00;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp       = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    bool                                              mVisible     = true;
    bool                                              mSmooth      = true;
    bool                                              mShowLen     = false;

    bool                                              mRuler;


    LineNode* mLineNode{nullptr};




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
    void setIsRuler(bool value);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    LinePropertiesModel *mLineProperties;
    UIHandle *mUiHandle = nullptr;
    bool mIsRuler;

};

#endif // LINEPROPERTIES_H
