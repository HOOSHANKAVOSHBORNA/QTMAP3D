#include "imageProperty.h"

#include <QQmlComponent>
#include <QQuickItem>

ImagePropertyModel::ImagePropertyModel(osgEarth::Annotation::ImageOverlay *image, MapItem *mapItem, QObject *parent):
    QObject(parent),
    mImage(image),
    mMapItem(mapItem){
}

QVector2D ImagePropertyModel::getLocation()
{
    return mLocation;
}

void ImagePropertyModel::setLocation(const QVector2D &location)
{
    if (mLocation == location)
        return;
    mLocation.setX(location.x());
    mLocation.setY(location.y());
    if (mImage){
        mImage->setCenter(static_cast<double>(location.x()),static_cast<double>(location.y()));
        emit positionToQmlChanged();
    }
}

QVector2D ImagePropertyModel::getTl()
{
    return mTl;
}

void ImagePropertyModel::setTl(const QVector2D &TL)
{
    if (mTl == TL)
        return;
    mTl.setX(TL.x());
    mTl.setY(TL.y());
    if (mImage){
        mImage->setUpperLeft(static_cast<double>(TL.x()),static_cast<double>(TL.y()));
        emit positionToQmlChanged();
    }
}

QVector2D ImagePropertyModel::getTr()
{
    return mTr;
}

void ImagePropertyModel::setTr(const QVector2D &TR)
{
    if (mTr == TR)
        return;
    mTr.setX(TR.x());
    mTr.setY(TR.y());
    if (mImage){
        mImage->setUpperRight(static_cast<double>(TR.x()),static_cast<double>(TR.y()));
        emit positionToQmlChanged();
    }
}

QVector2D ImagePropertyModel::getBr()
{

    return mBr;
}

void ImagePropertyModel::setBr(const QVector2D &BR)
{
    if (mBr == BR)
        return;
    mBr.setX(BR.x());
    mBr.setY(BR.y());
    if (mImage){
        mImage->setLowerRight(static_cast<double>(BR.x()),static_cast<double>(BR.y()));
        emit positionToQmlChanged();
    }
}

QVector2D ImagePropertyModel::getBl()
{

    return mBl;
}

void ImagePropertyModel::setBl(const QVector2D &BL)
{
    if (mBl == BL)
        return;
    mBl.setX(BL.x());
    mBl.setY(BL.y());
    if (mImage){
        mImage->setLowerLeft(static_cast<double>(BL.x()),static_cast<double>(BL.y()));
        emit positionToQmlChanged();
    }
}



void ImagePropertyModel::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    mImage = image;
    if (!mImage)
        return;
}




ImageProperty::ImageProperty(osgEarth::Annotation::ImageOverlay* image, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapItem, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
//    mImage(*image),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapItem, image](){
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mImageProperty = new ImagePropertyModel(image, mapItem);
            mItem->setProperty("imageProperty", QVariant::fromValue<ImagePropertyModel*>(mImageProperty));
    });
    comp->loadUrl(QUrl("qrc:/ImageProperty.qml"));
}

void ImageProperty::show()
{
    if(mItem)
    mUiHandle->propertiesShow(mItem);
}

void ImageProperty::hide()
{
    if(mItem)
    mUiHandle->propertiesHide(mItem);
}

void ImageProperty::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    if(mItem)
    mImageProperty->setImage(image);
}

void ImageProperty::setLocation(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperty->setLocation(tmp);
}

void ImageProperty::setTL(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperty->setTl(tmp);
}

void ImageProperty::setTR(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperty->setTr(tmp);
}

void ImageProperty::setBR(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperty->setBr(tmp);
}

void ImageProperty::setBL(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperty->setBl(tmp);
}

