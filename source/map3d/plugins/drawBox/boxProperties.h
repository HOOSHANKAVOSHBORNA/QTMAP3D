#ifndef BOXPROPERTIESMODEL_H
#define BOXPROPERTIESMODEL_H

#include <QObject>

#include "plugininterface.h"
#include "mapItem.h"
#include "box.h"
#include "property.h"

class BoxPropertiesModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   color          READ getColor         WRITE setColor        )
    Q_PROPERTY(QVector3D location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double    height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(double    width          READ getWidth         WRITE setWidth        )
    Q_PROPERTY(double    length         READ getLength        WRITE setLength       )
    Q_PROPERTY(int       opacity        READ getOpacity       WRITE setOpacity      )

public:
    BoxPropertiesModel(Box *box = nullptr, MapItem *mapItem = nullptr, QObject *parent = nullptr);

    //set color
    QString getColor() const;
    void setColor(const QString &fillcolor);
    //set location
    QVector3D  getLocation() const;
    void setLocation(const QVector3D  &location);
    //set height
    double getHeight() const;
    void setHeight(const double &boxHeight);
    //set Width
    double getWidth() const;
    void setWidth(const double &boxWidth);
    //set Length
    double getLength() const;
    void setLength(const double &boxLength);
    // set Opacity
    int getOpacity() const;
    void setOpacity(const int &transparency);
    // set relative
    bool getRelative() const;
    void setRelative(const bool &value);

    void setBox(Box *box);

signals:

    void circlePropertiesChanged( QVariant );
    void positionToQmlChanged();

private:
    QString   mColor ;
    QVector3D  mLocation ;
    double    mHeight ;
    double    mWidth  ;
    double    mLength ;
    int       mOpacity;
    bool      mRelative = false;

    Box *mBox;
    MapItem *mMapItem {nullptr};

};

class BoxProperties: public Property
{
    Q_OBJECT
public:
    BoxProperties(QQuickItem *parent=nullptr);
    void setFillColor(const QColor &color) override;
//    void setLenght(const QVector3D &status) override;
    void setLocationRelative(const bool &relative) override;
    void setLocation(const QVector3D &status) override;
    void setHeight(const double &height) override;
//    void setLenght(const double &lenght) override;
//    void setWidth( const double &width)  override;

    Box *getBox() const;
    void setBox(Box *newBox);

private:
    Box *mBox = nullptr;
};

#endif // BOXPROPERTIESMODEL_H
