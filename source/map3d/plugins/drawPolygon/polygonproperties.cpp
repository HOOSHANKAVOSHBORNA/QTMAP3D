#include "polygonproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "plugininterface.h"
#include "property.h"




PolygonPropertiesModel::PolygonPropertiesModel(QObject *parent) :
    QObject(parent)
{

}

QString PolygonPropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void PolygonPropertiesModel:: setFillColor(const QString &value){

    if(value == mFillcolor)
        return;
    mFillcolor = value;
    if(mPolygon){
        osgEarth::Color tmpColor = mPolygon->getFillColor();
        float A = tmpColor.a();
        tmpColor  = value.toStdString();
        tmpColor.a() = A;
        mPolygon->setFillColor(tmpColor);
    }
}


QString PolygonPropertiesModel::getLinecolor() const
{
    return mLinecolor;
}

void PolygonPropertiesModel:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    if(mPolygon){
        mPolygon->setLineColor(mLinecolor.toStdString());
    }
}

double PolygonPropertiesModel::getHeight() const
{
    return mHeight;
}

void PolygonPropertiesModel::setHeight(const double &value){
//    qDebug() << mHeight <<endl;
//    qDebug() << mPolygon->getHeight();
    if(std::abs(value - mHeight) < 1)
        return;
    mHeight = value;
    if(mPolygon){
        mPolygon->setHeight(static_cast<float>(value));
    }
}

int PolygonPropertiesModel::getTransparency() const
{
    return mTransparency;
}

void PolygonPropertiesModel::setTransparency(const int &value){
//    if(value == mTransparency)
//        return;
    mTransparency = value;
    if(mPolygon){
        float tempValue = value;
        osg::Vec4f tempColor = mPolygon->getFillColor();
        tempColor.a() = tempValue /100;
        mPolygon->setFillColor(osg::Vec4f(tempColor));
    }
}


int PolygonPropertiesModel::getLineOpacity() const
{
    return mLineOpacity;
}
void PolygonPropertiesModel::setLineOpacity(const int &value){
    if(value == mLineOpacity)
        return;
    mLineOpacity = value;
    if(mPolygon){
        float tempValue = value;
        osg::Vec4f tempColor = mPolygon->getLineColor();
        tempColor.a() = tempValue /100;
        mPolygon->setLineColor(osg::Vec4f(tempColor));
    }
}


int PolygonPropertiesModel::getClamp() const
{
    return mClamp;
}

void PolygonPropertiesModel::setClamp(int value){
    if(value == mClamp)
        return;
    mClamp = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(value);
    if(mPolygon){
        mPolygon->setClamp(mClamp);
    }
}


double PolygonPropertiesModel::getLineWidth() const
{
    return mLineWidth;
}

void PolygonPropertiesModel::setLineWidth(double line)
{
    mLineWidth = line;
    if (mPolygon)
        mPolygon->setLineWidth(static_cast<float>(mLineWidth));
}



void PolygonPropertiesModel::setPolygon(Polygon *polygon)
{
    mPolygon = polygon;
    if(!polygon){
        return;
    }
    osgEarth::Color tmpColor = mPolygon->getFillColor();
    float opacity = tmpColor.a();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity;
    mPolygon->setFillColor(tmpColor);
    osgEarth::Color tmplineColor = mPolygon->getLineColor();
    float lineopacity = tmplineColor.a();
    tmplineColor  = mLinecolor.toStdString();
    tmplineColor.a() = lineopacity;
    mPolygon->setLineColor(tmplineColor);
    mPolygon->setLineWidth(static_cast<float>(mLineWidth));
    setTransparency(mTransparency);
    mPolygon->setHeight(static_cast<float>(mHeight));
    mPolygon->setClamp(mClamp);
}

//PolygonProperties::PolygonProperties(QQmlEngine *engine, UIHandle *uiHandle,  QObject *parent) :
//    QObject(parent),
//    mQmlEngine(engine),
//    mUiHandle(uiHandle)
//{
//    //--show property window---------------------------------------------------------------------------------
//    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
//        if (comp->status() == QQmlComponent::Ready) {
//            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
//            mPolygonProperties = new PolygonPropertiesModel();
//            mItem->setProperty("polygonProperties", QVariant::fromValue<PolygonPropertiesModel*>(mPolygonProperties));
//        }
//    });
//    comp->loadUrl(QUrl("qrc:/PolygonProperties.qml"));
//    //--------------------------------------------------------------------------------------------------

//}

PolygonProperties::PolygonProperties(QQuickItem *parent):
    Property(parent)
{

}

void PolygonProperties::setFillColor(const QColor &color)
{
    setFillColorStatus(true);
    Property::setFillColor(color);
    //    mPolygon->setColor(color);
}





void PolygonProperties::setStrokeWidth(const double &opacity)
{
    setStrokeStatus(true);
    Property::setStrokeWidth(opacity);
}

void PolygonProperties::setStroke(const QColor &color)
{
    setStrokeStatus(true);
    Property::setStroke(color);

}

void PolygonProperties::setClamp(const int &clamp)
{
    setClampStatus(true);
    Property::setClamp(clamp);
}

void PolygonProperties::setHeight(const double &height)
{

}



Polygon *PolygonProperties::getPolygon() const
{
    return mPolygon;
}

void PolygonProperties::setPolygon(Polygon *newPolygon)
{
    mPolygon = newPolygon;
}



