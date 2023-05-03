#ifndef IMAGEPROPERTIESMODEL_H
#define IMAGEPROPERTIESMODEL_H

#include <QObject>
#include "plugininterface.h"
#include "mapcontroller.h"

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>

#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>

class DrawImage;
class ImagePropertiesModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector2D  location       READ getLocation      WRITE setLocation     NOTIFY positionToQmlChanged)
    Q_PROPERTY(double     height         READ getHeight        WRITE setHeight       )
    Q_PROPERTY(double     width          READ getWidth         WRITE setWidth        )


public:
    ImagePropertiesModel(osgEarth::Annotation::ImageOverlay *image = nullptr, MapController *mapcontroller = nullptr, QObject *parent = nullptr);

    QVector2D getLocation();
    void setLocation(const QVector2D  &location);
    //set height
    double getHeight() const;
    void setHeight(const double &imgHeight);
    //set Width
    double getWidth() const;
    void setWidth(const double &imgWidth);

    void setImage(osgEarth::Annotation::ImageOverlay *image);

signals:

    void imagePropertiesChanged( QVariant );
    void positionToQmlChanged();

private:
    QVector2D mLocation ;
    double    mHeight ;
    double    mWidth  ;


    osgEarth::Annotation::ImageOverlay *mImage{nullptr};
    MapController *mMapController {nullptr};
    DrawImage * mDrawImage{nullptr};

};

class ImageProperties: public QObject
{
    Q_OBJECT

public:
    ImageProperties(osgEarth::Annotation::ImageOverlay *image, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent = nullptr);

    void show();
    void hide();
    void setImage(osgEarth::Annotation::ImageOverlay *image);
    void setLocation(osg::Vec2d location);

private:
    QQmlEngine* mQmlEngine;
    QQuickItem* mItem{nullptr};
    osgEarth::Annotation::ImageOverlay mImage{nullptr};
    ImagePropertiesModel* mImageProperties{nullptr};
    UIHandle* mUiHandle;
};

#endif // IMAGEPROPERTIESMODEL_H