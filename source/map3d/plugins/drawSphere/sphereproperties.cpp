#include "sphereproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



SpherePropertiesModel::SpherePropertiesModel(SphereNode* sphereNode,MapController *mapController, QObject *parent) :
    QObject(parent),
    mSphereNode(sphereNode),
    mMapController(mapController)
{

       if (mSphereNode) {
           mLocation.setX(static_cast<float>(mSphereNode->getPosition().x()));
           mLocation.setY(static_cast<float>(mSphereNode->getPosition().y()));
           mLocation.setZ(static_cast<float>(mSphereNode->getPosition().z()));
//           mCenter.setX(static_cast<float>(mSphereNode->getCenter().x()));
//           mCenter.setY(static_cast<float>(mSphereNode->getCenter().y()));
//           mCenter.setZ(static_cast<float>(mSphereNode->getCenter().z()));
       }

//       QObject::connect(this,&SpherePropertiesModel::spherePropertiesChanged,this,&SpherePropertiesModel::spherePropertiesChangedToQML);

}

QString SpherePropertiesModel::getColor() const
{
    return mColor;
}
void SpherePropertiesModel:: setColor(const QString &value){
//    if(value == mColor)
//        return;
//    mColor = value;
//    osgEarth::Color tmpColor = mSphereNode->getColor();
//    float A = tmpColor.a();
//    tmpColor  = value.toStdString();
//    tmpColor.a() = A;
//    mSphereNode->setColor(tmpColor);
    if(value == mColor)
        return;
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
//    emit spherePropertiesChanged(Center, QVariant::fromValue<QVector3D>(value));
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
    if(std::abs(value - mRadius) < 1)
        return;
    mRadius = value;
    mSphereNode->setRadius(osgEarth::Distance(value));
}

int SpherePropertiesModel::getTransparency() const
{
    return mTransparency;
}
void SpherePropertiesModel::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    float tempValue = value;
    osg::Vec4f tempColor = mSphereNode->getColor();
    tempColor.a() = tempValue /100;
    mSphereNode->setColor(osg::Vec4f(tempColor));

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
    if(value == mRelative)
        return;
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


//    mRadius = sphere->getRadius().as(osgEarth::Units::METERS);
//    mSphereColor = QString::fromStdString(sphere->getColor().toHTML());
//    mSphereColor.remove(7,2);
//    mCenter.setX(sphere->getCenter().x());
//    mCenter.setY(sphere->getCenter().y());
//    mCenter.setZ(sphere->getCenter().z());
//    mRelative = sphere->getPosition().isRelative();
//    emit spherePropertiesChangedToQML();
}

SphereProperties::SphereProperties(SphereNode *sphere, QQmlEngine *engine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent):
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapcontroller, sphere](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mSphereProperties = new SpherePropertiesModel(sphere, mapcontroller);
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

void SphereProperties::setLocation(osgEarth::GeoPoint location)
{
    QVector3D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    tmp.setZ(static_cast<float>(location.z()));

    mSphereProperties->setLocation(tmp);
}
