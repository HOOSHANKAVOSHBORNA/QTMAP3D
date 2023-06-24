#include "imageProperties.h"
#include "drawImage.h"

#include <QQmlComponent>
#include <QQuickItem>

ImagePropertiesModel::ImagePropertiesModel(osgEarth::Annotation::ImageOverlay *image, MapItem *mapItem, QObject *parent):
    QObject(parent),
    mImage(image),
    mMapItem(mapItem)
{


}

QVector2D ImagePropertiesModel::getLocation()
{
    return mLocation;
}

void ImagePropertiesModel::setLocation(const QVector2D &location)
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

QVector2D ImagePropertiesModel::getTl()
{
    return mTl;
}

void ImagePropertiesModel::setTl(const QVector2D &TL)
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

QVector2D ImagePropertiesModel::getTr()
{
    return mTr;
}

void ImagePropertiesModel::setTr(const QVector2D &TR)
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

QVector2D ImagePropertiesModel::getBr()
{

    return mBr;
}

void ImagePropertiesModel::setBr(const QVector2D &BR)
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

QVector2D ImagePropertiesModel::getBl()
{

    return mBl;
}

void ImagePropertiesModel::setBl(const QVector2D &BL)
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



//double ImagePropertiesModel::getHeight() const
//{
//    return mHeight;
//}
//void ImagePropertiesModel::setHeight(const double &imgHeight)
//{
//    if (std::abs(mHeight - imgHeight) < 0.001)
//        return;
//    mHeight = imgHeight;
//    if (mImage){
//        mImage->setNorth(mImage->getCenter().y()+(mHeight/200));
//        mImage->setSouth(mImage->getCenter().y()-(mHeight/200));
//    }
////        mImage->setHeight(mHeight);
//}
//double ImagePropertiesModel::getWidth() const
//{
//    return mWidth;
//}
//void ImagePropertiesModel::setWidth(const double &imgWidth)
//{
//    if (std::abs(mWidth - imgWidth) < 0.001)
//        return;
//    mWidth = imgWidth;
//    if (mImage){
//            mImage->setEast(mImage->getCenter().x()+(mWidth/200));
//            mImage->setWest(mImage->getCenter().x()-(mWidth/200));
//    }
////        mDrawImage->setWidth(mWidth);
//}



void ImagePropertiesModel::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    mImage = image;
    if (!mImage)
        return;
//    mLocation.setX(mImage->getCenter().x());
//    mLocation.setY(mImage->getCenter().y());
//    if(mImage){
//        setHeight(mHeight);
//        setWidth(mWidth);
//    }
}




ImageProperties::ImageProperties(osgEarth::Annotation::ImageOverlay* image, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapItem, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
//    mImage(*image),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapItem, image](){
//        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mImageProperties = new ImagePropertiesModel(image, mapItem);
            mItem->setProperty("imageProperties", QVariant::fromValue<ImagePropertiesModel*>(mImageProperties));
//        }
    });
    comp->loadUrl(QUrl("qrc:/ImageProperty.qml"));
}

void ImageProperties::show()
{
    if(mItem)
    mUiHandle->propertiesShow(mItem);
}

void ImageProperties::hide()
{
    if(mItem)
    mUiHandle->propertiesHide(mItem);
}

void ImageProperties::setImage(osgEarth::Annotation::ImageOverlay *image)
{
    if(mItem)
    mImageProperties->setImage(image);
}

void ImageProperties::setLocation(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setLocation(tmp);
}

void ImageProperties::setTL(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setTl(tmp);
}

void ImageProperties::setTR(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setTr(tmp);
}

void ImageProperties::setBR(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setBr(tmp);
}

void ImageProperties::setBL(osg::Vec2d location)
{
    QVector2D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    mImageProperties->setBl(tmp);
}

