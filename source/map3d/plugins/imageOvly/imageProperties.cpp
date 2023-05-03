#include "imageProperties.h"
#include "drawImage.h"

#include <QQmlComponent>
#include <QQuickItem>

ImagePropertiesModel::ImagePropertiesModel(osgEarth::Annotation::ImageOverlay *image, MapController *mapcontroller, QObject *parent):
    QObject(parent),
    mImage(image),
    mMapController(mapcontroller)
{


}


QVector2D ImagePropertiesModel::getLocation()
{

//    mLocation.setX(static_cast<float>(mImage->getCenter().x()));
//    mLocation.setY(static_cast<float>(mImage->getCenter().y()));
    return mLocation;
}

void ImagePropertiesModel::setLocation(const QVector2D &location)
{
    if (mLocation == location)
        return;
    mLocation.setX(location.x());
    mLocation.setY(location.y());
    if (mImage){
//        osg::Vec2d tmpLocation = mImage->getCenter();
//        tmpLocation.x() = static_cast<double>(location.x());
//        tmpLocation.y() = static_cast<double>(location.y());
        mImage->setCenter(static_cast<double>(location.x()),static_cast<double>(location.y()));
        emit positionToQmlChanged();
    }
}



double ImagePropertiesModel::getHeight() const
{
    return mHeight;
}

void ImagePropertiesModel::setHeight(const double &imgHeight)
{
    if (std::abs(mHeight - imgHeight) < 0.001)
        return;
    mHeight = imgHeight;
    if (mImage){
        mImage->setNorth(mImage->getCenter().y()+(mHeight/200));
        mImage->setSouth(mImage->getCenter().y()-(mHeight/200));
    }

//        mImage->setHeight(mHeight);
}

double ImagePropertiesModel::getWidth() const
{
    return mWidth;
}

void ImagePropertiesModel::setWidth(const double &imgWidth)
{
    if (std::abs(mWidth - imgWidth) < 0.001)
        return;
    mWidth = imgWidth;
    if (mImage){
            mImage->setEast(mImage->getCenter().x()+(mWidth/200));
            mImage->setWest(mImage->getCenter().x()-(mWidth/200));
    }

//        mDrawImage->setWidth(mWidth);
}



void ImagePropertiesModel::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    mImage = image;
    if (!mImage)
        return;
//    mLocation.setX(mImage->getCenter().x());
//    mLocation.setY(mImage->getCenter().y());
    if(mImage){
        setHeight(mHeight);
        setWidth(mWidth);
    }

}




ImageProperties::ImageProperties(osgEarth::Annotation::ImageOverlay* image, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
//    mImage(*image),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapcontroller, image](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mImageProperties = new ImagePropertiesModel(image, mapcontroller);
            mItem->setProperty("imageProperties", QVariant::fromValue<ImagePropertiesModel*>(mImageProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/ImageProperty.qml"));
}

void ImageProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void ImageProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void ImageProperties::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    mImageProperties->setImage(image);
}

void ImageProperties::setLocation(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setLocation(tmp);
}

