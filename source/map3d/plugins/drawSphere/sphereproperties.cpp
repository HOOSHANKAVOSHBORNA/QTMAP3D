#include "sphereproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



SpherePropertiesModel::SpherePropertiesModel(MapController *mapController, QObject *parent) :
    QObject(parent),
    mMapController(mapController)
{

}

QString SpherePropertiesModel::getColor() const
{
    return mColor;
}
void SpherePropertiesModel:: setColor(const QString &value){

//    if(value == mColor)
//        return;
    mColor = value;
    if(mSphereNode){
        osgEarth::Color tmpColor = mSphereNode->getColor();
        float A = tmpColor.a();
        tmpColor  = value.toStdString();
        tmpColor.a() = A;
        mSphereNode->setColor(tmpColor);

    }
}

QVector3D SpherePropertiesModel::getLocation() const
{
    return mLocation;
}
void SpherePropertiesModel:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;

    osgEarth::GeoPoint tempLocation =  mSphereNode->getPosition();
    tempLocation.x() = static_cast<double>(value.x());
    tempLocation.y() = static_cast<double>(value.y());
    tempLocation.z() = static_cast<double>(value.z());
    mSphereNode->setPosition(tempLocation);
    emit spherePropertiesChangedToQML();
}


QVector3D SpherePropertiesModel::getCenter() const
{
    return mCenter;
}
void SpherePropertiesModel:: setCenter(const QVector3D &value){
    if(value == mCenter)
        return;
    mCenter = value;
    osg::Vec3f tempcenter = mSphereNode->getCenter();
    tempcenter.x() = value.x();
    tempcenter.y() = value.y();
    tempcenter.z() = value.z();
    mSphereNode->setCenter(tempcenter);

}

double SpherePropertiesModel::getRadius() const
{
    return mRadius;
}
void SpherePropertiesModel::setRadius(const double &value){
//    if(std::abs(value - mRadius) < 1)
//        return;
    mRadius = value;
    if(mSphereNode){ mSphereNode->setRadius(osgEarth::Distance(value));
    }
}

int SpherePropertiesModel::getTransparency() const
{
    return mTransparency;
}
void SpherePropertiesModel::setTransparency(const int &value){
//    if(value == mTransparency)
//        return;
    mTransparency = value;
    float tempValue = value;
    if(mSphereNode){
        osg::Vec4f tempColor = mSphereNode->getColor();
        tempColor.a() = tempValue /100;
        mSphereNode->setColor(osg::Vec4f(tempColor));
    }

}

int SpherePropertiesModel::getShape() const
{
    return mShape;
}
void SpherePropertiesModel::setShape(const int &value){
    if(value == mShape)
        return;
    mShape = value;
    mSphereNode->setSphereShape(SphereNode::SphereShape(value));
}

bool SpherePropertiesModel::getRelative() const
{
    return mRelative;
}
void SpherePropertiesModel::setRelative(const bool &value){
//    if(value == mRelative)
//        return;
    mRelative = value;
    osgEarth::GeoPoint tempLocation =  mSphereNode->getPosition();

    if(value == true)
    {
        tempLocation.makeRelative(mMapController->getMapNode()->getTerrain());
        mSphereNode->setPosition(tempLocation);
    }
    else if(value == false)
    {
        tempLocation.makeAbsolute(mMapController->getMapNode()->getTerrain());
        mSphereNode->setPosition(tempLocation);
    }

}

void SpherePropertiesModel::setSphere(SphereNode *sphere)
{
    mSphereNode = sphere;
    if(!sphere){
        return;
    }
    osgEarth::Color tmpColor = mSphereNode->getColor();
    float opacity = tmpColor.a();
    tmpColor  = mColor.toStdString();
    tmpColor.a() = opacity;
    mSphereNode->setColor(tmpColor);
    mSphereNode->setRadius(mRadius);
    setTransparency(mTransparency);
    setLocation(QVector3D(mSphereNode->getPosition().x(), mSphereNode->getPosition().y(), mSphereNode->getPosition().z()));

}

SphereProperties::SphereProperties(QQmlEngine *engine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent):
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapcontroller](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mSphereProperties = new SpherePropertiesModel(mapcontroller);
            mItem->setProperty("sphereProperties", QVariant::fromValue<SpherePropertiesModel*>(mSphereProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/SphereProperty.qml"));
}

void SphereProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void SphereProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void SphereProperties::setSphere(SphereNode *sphere)
{
    mSphereProperties->setSphere(sphere);
}

