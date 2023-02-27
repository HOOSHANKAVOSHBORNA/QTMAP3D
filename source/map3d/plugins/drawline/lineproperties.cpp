#include "lineproperties.h"
#include <QtDebug>
#include <QVector3D>



LineProperties::LineProperties(LineNode* lineNode, QObject *parent) :
    QObject(parent),
    mLineNode(lineNode)
{
       QObject::connect(this,&LineProperties::linePropertiesChanged,this,&LineProperties::linePropertiesChangedToQML);


//       mRadius = lineNode->getRadius().as(osgEarth::Units::METERS);
         mColor = QString::fromStdString(lineNode->getColor().toHTML());
         mColor.remove(7,2);
         mPointColor = QString::fromStdString(lineNode->getPointColor().toHTML());
         mPointColor.remove(7,2);

}

QString LineProperties::color() const
{
    return mColor;
}
void LineProperties:: setColor(const QString &value){
    if(value == mColor)
        return;
    mColor = value;
    emit linePropertiesChanged(Color , value);
    mLineNode->setColor(value.toStdString());

}

QString LineProperties::pointColor() const
{
    return mPointColor;
}
void LineProperties:: setPointColor(const QString &value){
    if(value == mPointColor)
        return;
    mPointColor = value;
    emit linePropertiesChanged(PointColor , value);
    mLineNode->setPointColor(value.toStdString());

}

float LineProperties::width() const
{
    return mWidth;
}
void LineProperties:: setWidth(const float &value){
    if(value == mWidth)
        return;
    mWidth = value;
    emit linePropertiesChanged(Width , value);
    mLineNode->setWidth(value);
}

float LineProperties::height() const
{
    return mHeight;
}
void LineProperties:: setHeight(const float &value){
    if(value == mHeight)
        return;
    mHeight = value;
    emit linePropertiesChanged(Height , value);
    mLineNode->setHeight(value);
}

float LineProperties::pointwidth() const
{
    return mPointwidth;
}
void LineProperties::setPointwidth(const float &value){
    if(value == mPointwidth)
        return;
    mPointwidth = value;
    emit linePropertiesChanged(Pointwidth, value);

    mLineNode->setPointWidth(value);
}

unsigned LineProperties::tesselation() const
{
    return mTesselation;
}
void LineProperties::setTesselation(const unsigned &value){
    if(value == mTesselation)
        return;
    mTesselation = value;
    emit linePropertiesChanged(Tesselation, value);


    mLineNode->setTessellation(value);

}

osgEarth::Symbology::AltitudeSymbol::Clamping  LineProperties::clamp() const
{
    return mClamp;
}
void LineProperties::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping  &value){
    if(value == mClamp)
        return;
    mClamp = value;
    emit linePropertiesChanged(Clamp, value);

    mLineNode->setClamp(value);
}

bool LineProperties::visible() const
{
    return mVisible;
}
void LineProperties::setVisible(const bool &value){
    if(value == mVisible)
        return;
    mVisible = value;
    emit linePropertiesChanged(Visible, value);

    mLineNode->setPointVisible(value);
}

bool LineProperties::smooth() const
{
    return mSmooth;
}
void LineProperties::setSmooth(const bool &value){
    if(value == mSmooth)
        return;
    mSmooth = value;
    emit linePropertiesChanged(Smooth, value);

    mLineNode->setSmooth(value);
}
