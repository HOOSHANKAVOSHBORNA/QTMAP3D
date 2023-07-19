#include "lineProperties.h"
#include <QtDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "plugininterface.h"
#include "property.h"



LinePropertiesModel::LinePropertiesModel(QObject *parent) :

    QObject(parent)
{

}

QString LinePropertiesModel::getColor() const
{
    return mColor;
}
void LinePropertiesModel:: setColor(const QString &value){
    if(value == mColor)
        return;
    mColor = value;
    if(mLineNode){
        osgEarth::Color tmpColor = mLineNode->getColor();
        float A = tmpColor.a();
        tmpColor = mColor.toStdString();
        tmpColor.a() = A;
        mLineNode->setColor(tmpColor);
    }else if(mMeasureHeight){
        osgEarth::Color tmpColor = mMeasureHeight->getColor();
        float A = tmpColor.a();
        tmpColor = mColor.toStdString();
        tmpColor.a() = A;
        mMeasureHeight->setColor(tmpColor);
    }

}

int LinePropertiesModel::getLineOpacity() const
{
    return mLineOpacity;
}
void LinePropertiesModel::setLineOpacity(const int &value){
    if(value == mLineOpacity)
        return;
    mLineOpacity = value;
    if(mLineNode){
        osgEarth::Color tmpColor = mLineNode->getColor();
        tmpColor.a() = static_cast<float>(value) / 100;
        mLineNode->setColor(tmpColor);
    }
    else if (mMeasureHeight) {
        osgEarth::Color tmpColor = mMeasureHeight->getColor();
        tmpColor.a() = static_cast<float>(value) / 100;
        mMeasureHeight->setColor(tmpColor);
    }
}


QString LinePropertiesModel::getPointColor() const
{
    return mPointColor;
}
void LinePropertiesModel:: setPointColor(const QString &value){
    if(value == mPointColor)
        return;
    mPointColor = value;
    if(mLineNode){
        mLineNode->setPointColor(value.toStdString());
    }

}


int LinePropertiesModel::getPointOpacity() const
{
    return mPointOpacity;
}
void LinePropertiesModel::setPointOpacity(const int &value){
    if(value == mPointOpacity)
        return;
    mPointOpacity = value;
    if(mLineNode){
        float tempValue = value;
        osg::Vec4f tempColor = mLineNode->getPointColor();
        tempColor.a() = tempValue /100;
        mLineNode->setPointColor(osg::Vec4f(tempColor));
    }
}

int LinePropertiesModel::getWidth() const
{
    return mWidth;
}
void LinePropertiesModel:: setWidth(const int &value){
    if(value == mWidth)
        return;
    mWidth =  value;
    if(mLineNode){
        mLineNode->setWidth(value);
    }else if(mMeasureHeight){
        mMeasureHeight->setWidth(value);
    }
}

float LinePropertiesModel::getHeight() const
{
    return mHeight;
}
void LinePropertiesModel:: setHeight(const float &value){
    if(value == mHeight)
        return;
    mHeight = value;
    if(mLineNode){
        mLineNode->setHeight(value);
    }
}

float LinePropertiesModel::getPointwidth() const
{
    return mPointwidth;
}
void LinePropertiesModel::setPointwidth(const float &value){
    if(value == mPointwidth)
        return;
    mPointwidth = value;
    if(mLineNode){
        mLineNode->setPointWidth(value);
    }
}

unsigned LinePropertiesModel::getTesselation() const
{
    return mTesselation;
}
void LinePropertiesModel::setTesselation(const unsigned &value){
    if(value == mTesselation)
        return;
    mTesselation = value;
    if(mLineNode){
        mLineNode->setTessellation(value);
    }
}

int LinePropertiesModel::getClamp() const
{
    return mClamp;
}
void LinePropertiesModel::setClamp(int value){
    if(value == mClamp)
        return;
    mClamp = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(value);
    if(mLineNode){
        mLineNode->setClamp(mClamp);
    }
}

bool LinePropertiesModel::getVisible() const
{
    return mVisible;
}
void LinePropertiesModel::setVisible(const bool &value){
    if(value == mVisible)
        return;
    mVisible = value;
    if(mLineNode){
        mLineNode->setPointVisible(value);
    }
}

bool LinePropertiesModel::getSmooth() const
{
    return mSmooth;
}
void LinePropertiesModel::setSmooth(const bool &value){
    if(value == mSmooth)
        return;
    mSmooth = value;
    if(mLineNode){
        mLineNode->setSmooth(value);
    }
}

bool LinePropertiesModel::getShowLen() const
{
    return mShowLen;
}
void LinePropertiesModel::setShowLen(const bool &value){
    if(value == mShowLen)
        return;
    mShowLen = value;
    if(mLineNode){
		mLineNode->setShowDistance(value);
    }
}

int LinePropertiesModel::getRuler() const
{
    return mRuler;
}

void LinePropertiesModel::setRuler(const int value)
{
    if(value != mRuler){
        mRuler = value;
//        emit linePropertiesChanged (value);
            emit rulerChanged();
    }
}

bool LinePropertiesModel::getShowBearing() const
{
    return mShowBearing;
}

void LinePropertiesModel::setShowBearing(const bool &bearing)
{
    if (bearing == mShowBearing){
        return;
    }
    mShowBearing = bearing;
    if(mLineNode){
        mLineNode->setShowBearing(bearing);
    }

}

void LinePropertiesModel::setLine(LineNode* linNode)
{
    mMeasureHeight = nullptr;
    mLineNode = linNode;
    if(!linNode){
        return;
    }
    mLineNode->setWidth(mWidth);
    mLineNode->setTessellation(mTesselation);
    mLineNode->setClamp(mClamp);
    mLineNode->setShowBearing(mShowBearing);
    mLineNode->setShowSlope(mShowSlope);
    osgEarth::Color tmpColorL = mLineNode->getColor();
    float opacity = mLineOpacity;
    tmpColorL  = mColor.toStdString();
    tmpColorL.a() = opacity / 100;
    mLineNode->setColor(tmpColorL);

    if(mRuler == 0)
    {
        mLineNode->setPointColor(mPointColor.toStdString());
        mLineNode->setPointWidth(mPointwidth);
        mLineNode->setSmooth(mSmooth);
        mLineNode->setPointVisible(mVisible);
        mLineNode->setHeight(mHeight);
		mLineNode->setShowDistance(mShowLen);

    }
    else if(mRuler == 1)
    {
        mLineNode->setWidth(mWidth);
        mLineNode->setShowDistance(true);
    }

    else if(mRuler == 2)
    {
        mLineNode->setWidth(mWidth);
    }
    else if(mRuler == 3)
    {
        mLineNode->setWidth(mWidth);
        mLineNode->setShowSlope(true);
    }
}

void LinePropertiesModel::setMeasureHeight(MeasureHeight *measureHeight)
{
    mLineNode = nullptr;
    mMeasureHeight = measureHeight;
    if(!measureHeight){
        return;
    }
    osgEarth::Color tmpColorH = mMeasureHeight->getColor();
    float opacity = mLineOpacity;
    tmpColorH  = mColor.toStdString();
    tmpColorH.a() = opacity / 100;
    mMeasureHeight->setColor(tmpColorH);
    mMeasureHeight->setWidth(mWidth);
}

bool LinePropertiesModel::getShowSlope() const
{
    return mShowSlope;
}

void LinePropertiesModel::setShowSlope(bool showSlope)
{
    if (showSlope == mShowSlope){
        return;
    }
    mShowSlope = showSlope;
    if(mLineNode){
        mLineNode->setShowSlope(showSlope);
    }
}

//LineProperties::LineProperties(QQmlEngine *engine,UIHandle *muiHandle, QObject *parent ):
//    QObject(parent),
//    mQmlEngine(engine),
//    mUiHandle(muiHandle)
//{
//    //--show property window---------------------------------------------------------------------------------
//    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
//        if (comp->status() == QQmlComponent::Ready) {
//            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
//            mLineProperties = new LinePropertiesModel();
//            mItem->setProperty("lineProperties", QVariant::fromValue<LinePropertiesModel*>(mLineProperties));
//        }
//    });
//    comp->loadUrl(QUrl("qrc:/LineProperty.qml"));
//    //--------------------------------------------------------------------------------------------------

//}

LineProperties::LineProperties(QQuickItem *parent):
    Property(parent)
{

}

void LineProperties::setFillColor(const QColor &color)
{
    setFillColorStatus(true);
    Property::setFillColor(color);
    //    mLine->setColor(color);
}





void LineProperties::setStrokeWidth(const double &opacity)
{
    setStrokeStatus(true);
    Property::setStrokeWidth(opacity);
}

void LineProperties::setStroke(const QColor &color)
{
    setStrokeStatus(true);
    Property::setStroke(color);

}

void LineProperties::setPoints(const QColor &point)
{
    setPointsStatus(true);
    Property::setPoints(point);

}

void LineProperties::setPointsWidth(const double &point)
{
    setPointsStatus(true);
    Property::setPointsWidth(point);
}

void LineProperties::setPointsSmooth(const bool &point)
{
    setPointsStatus(true);
    Property::setPointsSmooth(point);
}

void LineProperties::setBearing(const bool &bearing)
{
    setBearingStatus(true);
    Property::setBearing(bearing);
}

void LineProperties::setShowSlop(const bool &slop)
{
    setShowSlopStatus(true);
    Property::setShowSlop(slop);
}

void LineProperties::setTesselation(const double &tesselation)
{
    setTesselationStatus(true);
    Property::setTesselation(tesselation);
}





void LineProperties::setWidth(const double &width)
{
    setWidthStatus(true);
    Property::setWidth(width);
}

void LineProperties::setClamp(const double &clamp)
{
    setClampStatus(true);
    Property::setClamp(clamp);
}

void LineProperties::setShowLen(const bool &clamp)
{
    setShowLenStatus(true);
    Property::setShowLen(clamp);
}


LineNode *LineProperties::getLine() const
{
    return mLineNode;
}

void LineProperties::setLine(LineNode *newLine)
{
    mLineNode = newLine;
}
