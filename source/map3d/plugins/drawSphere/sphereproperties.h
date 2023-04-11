#ifndef SPHEREPROPERTIES_H
#define SPHEREPROPERTIES_H

#include "sphereNode.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>

class SpherePropertiesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ getColor         WRITE setColor        NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(QVector3D center         READ getCenter        WRITE setCenter       NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(double    radius         READ getRadius        WRITE setRadius       NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(int       transparency   READ getTransparency  WRITE setTransparency NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(int       shape          READ getShape         WRITE setShape        NOTIFY spherePropertiesChangedToQML)
    Q_PROPERTY(bool      relative       READ getRelative      WRITE setRelative     NOTIFY spherePropertiesChangedToQML)


public:



    SpherePropertiesModel(MapController *mapController = nullptr, QObject *parent = nullptr);
    //set color
    QString getColor() const;
    void setColor(const QString &color);
    //set location
    QVector3D getLocation() const;
    void setLocation(const QVector3D &location);
    //set center
    QVector3D getCenter() const;
    void setCenter(const QVector3D &center);
    // set radius
    double getRadius() const;
    void setRadius(const double &radius);
    // set transparency
    int getTransparency() const;
    void setTransparency(const int &transparency);
    // set Type
    int getShape() const;
    void setShape(const int &type);
    // set relative
    bool getRelative() const;
    void setRelative(const bool &relative);

    void setSphere(SphereNode *sphere);

signals:

    void spherePropertiesChanged( QVariant );
    void spherePropertiesChangedToQML();


private:
    QString     mColor  ;
    QVector3D   mLocation;
    QVector3D   mCenter;
    double      mRadius ;
    int         mTransparency ;
    int         mShape;
    bool        mRelative;


    SphereNode* mSphereNode{nullptr};
    MapController* mMapController{nullptr};


public slots:
};

class SphereProperties: public QObject
{
    Q_OBJECT
public:
    SphereProperties(QQmlEngine *engine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent = nullptr);
    void show();
    void hide();
    void setSphere(SphereNode* sphere);
private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem;
    SpherePropertiesModel* mSphereProperties;
    UIHandle* mUiHandle;
};


#endif // SPHEREPROPERTIES_H
