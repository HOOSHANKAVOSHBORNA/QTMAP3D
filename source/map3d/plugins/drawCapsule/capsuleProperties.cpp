#include "capsuleProperties.h"

#include <QQmlComponent>
#include <QQuickItem>

CapsulePropertiesModel::CapsulePropertiesModel(Capsule *Capsule, MapItem *mapItem, QObject *parent):
    QObject(parent),
    mCapsule(Capsule),
    mMapItem(mapItem)
{
}

QString CapsulePropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void CapsulePropertiesModel::setFillColor(const QString &fillcolor)
{
    if (mFillcolor == fillcolor)
        return;
    mFillcolor = fillcolor;
    if (mCapsule) {
        osgEarth::Color tmpColor = mCapsule->getColor();
        float A = tmpColor.a();
        tmpColor = mFillcolor.toStdString();
        tmpColor.a() = A;
        mCapsule->setColor(tmpColor);
    }
}

QVector3D CapsulePropertiesModel::getLocation() const
{
    return mLocation;
}

void CapsulePropertiesModel::setLocation(const QVector3D &location)
{
    if (mLocation == location)
        return;
    mLocation = location;
    if (mCapsule){
        osgEarth::GeoPoint tmpLocation = mCapsule->getPosition();
        tmpLocation.x() = static_cast<double>(location.x());
        tmpLocation.y() = static_cast<double>(location.y());
        tmpLocation.z() = static_cast<double>(location.z()+mCapsule->getHeight().getValue()/2);
        mCapsule->setPosition(tmpLocation);
        emit positionToQmlChanged();
    }
}

double CapsulePropertiesModel::getRadius() const
{
    return mRadius;
}

void CapsulePropertiesModel::setRadius(const double &radius)
{
    if (std::abs(mRadius - radius) < 0.001)
        return;
    mRadius = radius;
    if (mCapsule)
        mCapsule->setRadius(mRadius);
}

double CapsulePropertiesModel::getHeight() const
{
    return mHeight;
}

void CapsulePropertiesModel::setHeight(const double &CapsuleHeight)
{
    if (std::abs(mHeight - CapsuleHeight) < 0.001)
        return;
    mHeight = CapsuleHeight;
    if (mCapsule)
        mCapsule->setHeight(mHeight);
}

int CapsulePropertiesModel::getTransparency() const
{
    return mTransparency;
}

void CapsulePropertiesModel::setTransparency(const int &transparency)
{
    if (mTransparency == transparency)
        return;
    mTransparency = transparency;
    if (mCapsule) {
        osgEarth::Color tmpColor = mCapsule->getColor();
        tmpColor.a() = static_cast<float>(transparency) / 100;
        mCapsule->setColor(tmpColor);
    }
}

void CapsulePropertiesModel::setCapsule(Capsule *Capsule)
{
    mCapsule = Capsule;
    if (!mCapsule)
        return;
    osgEarth::Color tmpColor = mCapsule->getColor();
    float opacity = mTransparency;
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity / 100;
    mCapsule->setColor(tmpColor);
    mCapsule->setHeight(mHeight);
    mCapsule->setRadius(mRadius);
}

void CapsulePropertiesModel::setRelative(const bool &value){
        if(value == mRelative)
            return;
    mRelative = value;
    if(mCapsule){
        osgEarth::GeoPoint tempLocation =  mCapsule->getPosition();
        if(value == true )
        {
            tempLocation.makeRelative(mMapItem->getMapNode()->getTerrain());
            mCapsule->setPosition(tempLocation);
        }
        else if(value == false )
        {
            tempLocation.makeAbsolute(mMapItem->getMapNode()->getTerrain());
            mCapsule->setPosition(tempLocation);
        }
    }
}


//CapsuleProperties::CapsuleProperties(Capsule* Capsule, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapItem, QObject *parent) :
//    QObject(parent),
//    mQmlEngine(qmlEngine),
//    mUiHandle(uiHandle)
//{
//    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapItem, Capsule](){
//        if (comp->status() == QQmlComponent::Ready) {
//            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
//            mCapsuleProperties = new CapsulePropertiesModel(Capsule, mapItem);
//            mItem->setProperty("capsuleProperties", QVariant::fromValue<CapsulePropertiesModel*>(mCapsuleProperties));
//        }
//    });
//    comp->loadUrl(QUrl("qrc:/CapsuleProperty.qml"));
//}

CapsuleProperties::CapsuleProperties(QQuickItem *parent):
    Property(parent)
{

}

void CapsuleProperties::setFillColor(const QColor &color)
{
    setFillColorStatus(true);
    Property::setFillColor(color);
//    mCapsule->setColor(color);
}

void CapsuleProperties::setRadius(const double &radius)
{
    setRadiusStatus(true);
    Property::setRadius(radius);
}
void CapsuleProperties::setLocationRelative(const bool &relative)
{
    setLocationRelative(true);
    Property::setLocationRelative(relative);
}



void CapsuleProperties::setLocation(const QVector3D &status)
{
    setLocationStatus(true);
    Property::setLocation(status);


}

void CapsuleProperties::setHeight(const double &height)
{
    setHeightStatus(true);
    Property::setHeight(height);
}


Capsule *CapsuleProperties::getCapsule() const
{
    return mCapsule;
}

//void CapsuleProperties::setCapsule(Capsule *newCapsule)
//{
//    mCapsule = newCapsule;
//}

