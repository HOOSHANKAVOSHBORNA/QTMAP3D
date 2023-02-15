#include "sphereproperties.h"
#include <QtDebug>
#include <QVector3D>



SphereProperties::SphereProperties(SphereNode* sphereNode,MapController *mapController, QObject *parent) :
    QObject(parent),
    mSphereNode(sphereNode)
{
       mMapController = mapController;

       mRadius = sphereNode->getRadius().as(osgEarth::Units::METERS);
       mSphereColor = QString::fromStdString(sphereNode->getColor().toHTML());
       mSphereColor.remove(7,2);
       mLocation.setX(sphereNode->getPosition().x());
       mLocation.setY(sphereNode->getPosition().y());
       mLocation.setZ(sphereNode->getPosition().z());
       mCenter.setX(sphereNode->getCenter().x());
       mCenter.setY(sphereNode->getCenter().y());
       mCenter.setZ(sphereNode->getCenter().z());
       mRelative = sphereNode->getPosition().isRelative();
       QObject::connect(this,&SphereProperties::spherePropertiesChanged,this,&SphereProperties::spherePropertiesChangedToQML);

}

QString SphereProperties::color() const
{
    return mSphereColor;
}
void SphereProperties:: setColor(const QString &value){
    if(value == mSphereColor)
        return;
    mSphereColor = value;
    emit spherePropertiesChanged(Color , value);

    osgEarth::Color tmpColor = mSphereNode->getColor();
    float A = tmpColor.a();
    tmpColor  = value.toStdString();
    tmpColor.a() = A;
    mSphereNode->setColor(tmpColor);

}

QVector3D SphereProperties::location() const
{
    return mLocation;
}
void SphereProperties:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    //emit spherePropertiesChanged(Location, QVariant::fromValue<QVector3D>(value));
    emit spherePropertiesChanged(Location , value);

    osgEarth::GeoPoint tempLocation =  mSphereNode->getPosition();
    tempLocation.x() = value.x();
    tempLocation.y() = value.y();
    tempLocation.z() = value.z();


    mSphereNode->setPosition(tempLocation);
}


QVector3D SphereProperties::center() const
{
    return mCenter;
}
void SphereProperties:: setCenter(const QVector3D &value){
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

double SphereProperties::radius() const
{
    return mRadius;
}
void SphereProperties::setRadius(const double &value){
    if(value == mRadius)
        return;
    mRadius = value;
    emit spherePropertiesChanged(Radius, value);

    mSphereNode->setRadius(osgEarth::Distance(value));
}

int SphereProperties::transparency() const
{
    return mTransparency;
}
void SphereProperties::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    emit spherePropertiesChanged(Transparency, value);

    float tempValue = value;
    osg::Vec4f tempColor = mSphereNode->getColor();
    tempColor.a() = tempValue /100;

    mSphereNode->setColor(osg::Vec4f(tempColor));

}

int SphereProperties::shape() const
{
    return mShape;
}
void SphereProperties::setShape(const int &value){
    if(value == mShape)
        return;
    mShape = value;
    emit spherePropertiesChanged(Shape, value);

    mSphereNode->setSphereShape(SphereNode::SphereShape(value));
}

bool SphereProperties::relative() const
{
    return mRelative;
}
void SphereProperties::setRelative(const bool &value){
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
