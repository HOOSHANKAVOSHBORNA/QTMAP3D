#ifndef IMAGEPROPERTYMODEL_H
#define IMAGEPROPERTYMODEL_H

#include <QObject>
#include "plugininterface.h"
#include "mapItem.h"

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>

//#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>

class DrawImage;
class ImagePropertyModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector2D  location    READ getLocation       WRITE setLocation       NOTIFY positionToQmlChanged)
    Q_PROPERTY(QVector2D  tl          READ getTl             WRITE setTl             NOTIFY positionToQmlChanged)
    Q_PROPERTY(QVector2D  tr          READ getTr             WRITE setTr             NOTIFY positionToQmlChanged)
    Q_PROPERTY(QVector2D  br          READ getBr             WRITE setBr             NOTIFY positionToQmlChanged)
    Q_PROPERTY(QVector2D  bl          READ getBl             WRITE setBl             NOTIFY positionToQmlChanged)



public:
    ImagePropertyModel(osgEarth::Annotation::ImageOverlay *image = nullptr, MapItem *mapItem = nullptr, QObject *parent = nullptr);

    QVector2D getLocation();
    void setLocation(const QVector2D  &location);
    QVector2D getTl();
    void setTl(const QVector2D  &location);
    QVector2D getTr();
    void setTr(const QVector2D  &location);
    QVector2D getBr();
    void setBr(const QVector2D  &location);
    QVector2D getBl();
    void setBl(const QVector2D  &location);


    void setImage(osgEarth::Annotation::ImageOverlay *image);

signals:

    void imagePropertyChanged( QVariant );
    void positionToQmlChanged();

private:
    QVector2D mLocation ;
    QVector2D mTl ;
    QVector2D mTr ;
    QVector2D mBr ;
    QVector2D mBl ;



    osgEarth::Annotation::ImageOverlay *mImage{nullptr};
    MapItem *mMapItem {nullptr};
    DrawImage * mDrawImage{nullptr};

};

class ImageProperty: public QObject
{
    Q_OBJECT

public:
    ImageProperty(osgEarth::Annotation::ImageOverlay *image, QQmlEngine *qmlEngine, MapItem *mapItem, QObject *parent = nullptr);

    void show();
    void hide();
    void setImage(osgEarth::Annotation::ImageOverlay *image);
    void setLocation(osg::Vec2d location);
    void setTL(osg::Vec2d location);
    void setTR(osg::Vec2d location);
    void setBR(osg::Vec2d location);
    void setBL(osg::Vec2d location);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem{nullptr};
    osgEarth::Annotation::ImageOverlay mImage{nullptr};
    ImagePropertyModel* mImageProperty{nullptr};
};

#endif // IMAGEPROPERTYMODEL_H
