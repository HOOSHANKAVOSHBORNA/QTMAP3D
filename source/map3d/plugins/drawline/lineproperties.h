#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include "linenode.h"
#include "measureHeight.h"
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
    Q_PROPERTY(int       lineOpacity    READ getLineOpacity   WRITE setLineOpacity  )
    Q_PROPERTY(QString   pointColor     READ getPointColor    WRITE setPointColor   )
    Q_PROPERTY(int       pointOpacity   READ getPointOpacity  WRITE setPointOpacity )
    Q_PROPERTY(QString   width          READ getWidth         WRITE setWidth        )
    Q_PROPERTY(float     pointwidth     READ getPointwidth    WRITE setPointwidth   )
    Q_PROPERTY(float     height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(unsigned  tesselation    READ getTesselation   WRITE setTesselation  )
    Q_PROPERTY(int       clamp          READ getClamp         WRITE setClamp        )
    Q_PROPERTY(bool      visible        READ getVisible       WRITE setVisible      )
    Q_PROPERTY(bool      smooth         READ getSmooth        WRITE setSmooth       )
    Q_PROPERTY(bool      showLen        READ getShowLen       WRITE setShowLen      )
    Q_PROPERTY(int       ruler          READ getRuler         WRITE setRuler        NOTIFY rulerChanged )

public:

    LinePropertiesModel( QObject *parent = nullptr);
    //set color
    QString getColor() const;
    void setColor(const QString &color);
    //set color
    int getLineOpacity() const;
    void setLineOpacity(const int &lineOpacity);
    //set color
    QString getPointColor() const;
    void setPointColor(const QString &pointColor);
    //set color
    int getPointOpacity() const;
    void setPointOpacity(const int &pointOpacity);
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

    int  getRuler() const;
    void setRuler(const int ruler);

    bool getShowBearing() const;
    void setShowBearing(const bool &bearing);

    void setLine(LineNode *linNode) ;
    void setMeasureHeight(MeasureHeight *measureHeight);

signals:

    void linePropertiesChanged(  QVariant );
    void rulerChanged();


private:
    QString                                           mColor       = "#91001d";
    int                                               mLineOpacity = 100 ;
    int                                               mPointOpacity= 100 ;
    QString                                           mPointColor  = "#001191";
    int                                               mWidth       = 10.00;
    float                                             mHeight      = 0;
    float                                             mPointwidth  = 10.00;
    unsigned                                          mTesselation =10.00;
    osgEarth::Symbology::AltitudeSymbol::Clamping     mClamp       = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    bool                                              mVisible     = true;
    bool                                              mSmooth      = true;
    bool                                              mShowLen     = false;

    int                                              mRuler;


    LineNode* mLineNode{nullptr};
    MeasureHeight* mMeasureHeight{nullptr};





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
    void setMeasureHeight(MeasureHeight *measureHeight);
    void setIsRuler(int value);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    LinePropertiesModel *mLineProperties;
    UIHandle *mUiHandle = nullptr;
    int mIsRuler;

};

#endif // LINEPROPERTIES_H
