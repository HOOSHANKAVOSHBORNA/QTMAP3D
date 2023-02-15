#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>



CircleProperties::CircleProperties(Circle* circle, MapController *mapController, QObject *parent) :
    QObject(parent),
    mCircle(circle)
{
    QObject::connect(this,&CircleProperties::circlePropertiesChanged,this,&CircleProperties::circlePropertiesChangedToQML);
    mMapController = mapController;


//    mFillcolor = QString::fromStdString(circle->getColor().toHTML());
//    mFillcolor.remove(7,2);
//    mCircleHeight = circle->getCircleHeight();
    mLocation.setX(circle->getPosition().x());
    mLocation.setY(circle->getPosition().y());
    mLocation.setZ(circle->getPosition().z());
    mRelative = circle->getPosition().isRelative();
    mRadius = circle->getRadius().as(osgEarth::Units::METERS);
    mArcstart = circle->getArcStart().getValue();
    mArcend = circle->getArcEnd().getValue();
//    mClamp = circle->getClamp();


}

QString CircleProperties::fillcolor() const
{
    return mFillcolor;
}
void CircleProperties:: setFillColor(const QString &value){
    if(value == mFillcolor)
        return;
    mFillcolor = value;
    emit circlePropertiesChanged(FillColor , value);

//    osgEarth::Color tmpColor = mCircle->getStyle();
//    float A = tmpColor.a();
//    tmpColor  = value.toStdString();
//    tmpColor.a() = A;
//    mCircle->setColor(tmpColor);



}

QString CircleProperties::linecolor() const
{
    return mLinecolor;
}
void CircleProperties:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    emit circlePropertiesChanged(LineColor , value);
}


QVector3D CircleProperties::location() const
{
    return mLocation;
}
void CircleProperties:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    emit circlePropertiesChanged(Location , value);

    osgEarth::GeoPoint tempLocation =  mCircle->getPosition();
    tempLocation.x() = value.x();
    tempLocation.y() = value.y();
    tempLocation.z() = value.z();
    mCircle->setPosition(tempLocation);
}


double CircleProperties::radius() const
{
    return mRadius;
}
void CircleProperties::setRadius(const double &value){
    if(value == mRadius)
        return;
    mRadius = value;
    emit circlePropertiesChanged(Radius, value);

    mCircle->setRadius(osgEarth::Distance(value));
}


double CircleProperties::circleheight() const
{
    return mCircleHeight;
}
void CircleProperties::setCircleHeight(const double &value){
    if(value == mCircleHeight)
        return;
    mCircleHeight = value;
    emit circlePropertiesChanged(CircleHeight, value);

    mCircle->setCircleHeight(value);
}


int CircleProperties::transparency() const
{
    return mTransparency;
}
void CircleProperties::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    emit circlePropertiesChanged(Transparency, value);
//    float tempValue = value;
//    osg::Vec4f tempColor = mCircle->getColor();
//    tempColor.a() = tempValue /100;
//    mCircle->setColor(osg::Vec4f(tempColor));

}


double CircleProperties::arcstart() const
{
    return mArcstart;
}
void CircleProperties::setArcstart(const double &value){
    if(value == mArcstart)
        return;
    mArcstart = value;
    emit circlePropertiesChanged(ArcStart, value);

    mCircle->setArcStart(value);
}

double CircleProperties::arcend() const
{
    return mArcend;
}
void CircleProperties::setArcend(const double &value){
    if(value == mArcend)
        return;
    mArcend = value;
    emit circlePropertiesChanged(ArcEnd, value);

    mCircle->setArcEnd(value);
}


int CircleProperties::clamp() const
{
    return mClamp;
}
void CircleProperties::setClamp(const int &value){
    if(value == mClamp)
        return;
    mClamp = value;
    emit circlePropertiesChanged(Clamp, value);

    mCircle->setClamp(value);
}


bool CircleProperties::relative() const
{
    return mRelative;
}
void CircleProperties::setRelative(const bool &value){
    if(value == mRelative)
        return;
    mRelative = value;
    emit circlePropertiesChanged(Relative, value);
    osgEarth::GeoPoint tempLocation =  mCircle->getPosition();

    if(value == true)
    {
        tempLocation.makeRelative(mMapController->getMapNode()->getTerrain());
        mCircle->setPosition(tempLocation);
    }
    else if(value == false)
    {
        tempLocation.makeAbsolute(mMapController->getMapNode()->getTerrain());
        mCircle->setPosition(tempLocation);
    }
}

