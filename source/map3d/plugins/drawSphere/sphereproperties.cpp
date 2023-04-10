#include "sphereproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



SpherePropertiesModel::SpherePropertiesModel(SphereNode* sphereNode,MapController *mapController, QObject *parent) :
    QObject(parent),
    mSphereNode(sphereNode)
{
       mMapController = mapController;

//       mRadius = sphereNode->getRadius().as(osgEarth::Units::METERS);
//       mSphereColor = QString::fromStdString(sphereNode->getColor().toHTML());
//       mSphereColor.remove(7,2);
//       mLocation.setX(sphereNode->getPosition().x());
//       mLocation.setY(sphereNode->getPosition().y());
//       mLocation.setZ(sphereNode->getPosition().z());
//       mCenter.setX(sphereNode->getCenter().x());
//       mCenter.setY(sphereNode->getCenter().y());
//       mCenter.setZ(sphereNode->getCenter().z());
//       mRelative = sphereNode->getPosition().isRelative();
       QObject::connect(this,&SpherePropertiesModel::spherePropertiesChanged,this,&SpherePropertiesModel::spherePropertiesChangedToQML);

}

QString SpherePropertiesModel::color() const
{
    return mSphereColor;
}
void SpherePropertiesModel:: setColor(const QString &value){
    if(value == mSphereColor)
        return;
    mSphereColor = value;

    osgEarth::Color tmpColor = mSphereNode->getColor();
    float A = tmpColor.a();
    tmpColor  = value.toStdString();
    tmpColor.a() = A;
    mSphereNode->setColor(tmpColor);
    emit spherePropertiesChangedToQML();

}

QVector3D SpherePropertiesModel::location() const
{
    return mLocation;
}
void SpherePropertiesModel:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    //emit spherePropertiesChanged(Location, QVariant::fromValue<QVector3D>(value));

    osgEarth::GeoPoint tempLocation =  mSphereNode->getPosition();
    tempLocation.x() = value.x();
    tempLocation.y() = value.y();
    tempLocation.z() = value.z();


    mSphereNode->setPosition(tempLocation);
    emit spherePropertiesChanged(Location , value);
}


QVector3D SpherePropertiesModel::center() const
{
    return mCenter;
}
void SpherePropertiesModel:: setCenter(const QVector3D &value){
    if(value == mCenter)
        return;
    mCenter = value;
//    emit spherePropertiesChanged(Center, QVariant::fromValue<QVector3D>(value));
    emit spherePropertiesChanged(Center , value);
    osg::Vec3f tempcenter = mSphereNode->getCenter();
    tempcenter.x() = value.x();
    tempcenter.y() = value.y();
    tempcenter.z() = value.z();
    mSphereNode->setCenter(tempcenter);

}

double SpherePropertiesModel::radius() const
{
    return mRadius;
}
void SpherePropertiesModel::setRadius(const double &value){
    if(value == mRadius)
        return;
    mRadius = value;
    emit spherePropertiesChanged(Radius, value);

    mSphereNode->setRadius(osgEarth::Distance(value));
}

int SpherePropertiesModel::transparency() const
{
    return mTransparency;
}
void SpherePropertiesModel::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    emit spherePropertiesChanged(Transparency, value);

    float tempValue = value;
    osg::Vec4f tempColor = mSphereNode->getColor();
    tempColor.a() = tempValue /100;

    mSphereNode->setColor(osg::Vec4f(tempColor));

}

int SpherePropertiesModel::shape() const
{
    return mShape;
}
void SpherePropertiesModel::setShape(const int &value){
    if(value == mShape)
        return;
    mShape = value;
    emit spherePropertiesChanged(Shape, value);

    mSphereNode->setSphereShape(SphereNode::SphereShape(value));
}

bool SpherePropertiesModel::relative() const
{
    return mRelative;
}
void SpherePropertiesModel::setRelative(const bool &value){
    if(value == mRelative)
        return;
    mRelative = value;
    emit spherePropertiesChanged(Relative, value);
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
    tmpColor  = mSphereColor.toStdString();
    tmpColor.a() = opacity;
    mSphereNode->setColor(tmpColor);
    mSphereNode->setRadius(mRadius);
    setTransparency(mTransparency);
    mSphereNode->setRadius(mRadius);

    mRadius = sphere->getRadius().as(osgEarth::Units::METERS);
    mSphereColor = QString::fromStdString(sphere->getColor().toHTML());
    mSphereColor.remove(7,2);
    mCenter.setX(sphere->getCenter().x());
    mCenter.setY(sphere->getCenter().y());
    mCenter.setZ(sphere->getCenter().z());
    mRelative = sphere->getPosition().isRelative();
    emit spherePropertiesChangedToQML();
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
