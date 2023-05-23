#include "lineproperties.h"
#include <QtDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "plugininterface.h"



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
        mLineNode->setColor(value.toStdString());
    }else if(mMeasureHeight){
        mMeasureHeight->setColor(value.toStdString());
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
        float tempValue = value;
        mOpColor = mLineNode->getColor();
        mOpColor.a() = tempValue /100;
        mLineNode->setColor(osg::Vec4f(mOpColor));
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
void LinePropertiesModel:: setWidth(const QString &value){
    if(value.toInt() == mWidth)
        return;
    mWidth =  value.toInt() ;
    if(mLineNode){
        mLineNode->setWidth(value.toInt());
    }else if(mMeasureHeight){
        mMeasureHeight->setWidth(value.toInt());
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
    mLineNode->setColor(mColor.toStdString());
    mLineNode->setWidth(mWidth);
    mLineNode->setTessellation(mTesselation);
    mLineNode->setClamp(mClamp);
    mLineNode->setShowBearing(mShowBearing);
    mLineNode->setShowSlope(mShowSlope);
    mLineNode->setColor(mOpColor);


    if(mRuler == 0)
    {
        mLineNode->setPointColor(mPointColor.toStdString());
        mLineNode->setPointWidth(mPointwidth);
        mLineNode->setSmooth(mSmooth);
        mLineNode->setPointVisible(mVisible);
        mLineNode->setHeight(mHeight);
		mLineNode->setShowDistance(mShowLen);
        mLineNode->setColor(mOpColor);
    }
    else if(mRuler == 1)
    {
        mLineNode->setColor(mColor.toStdString());
        mLineNode->setWidth(mWidth);
        mLineNode->setShowDistance(true);
        mLineNode->setColor(mOpColor);
    }

    else if(mRuler == 2)
    {
        mLineNode->setColor(mColor.toStdString());
        mLineNode->setWidth(mWidth);
        mLineNode->setColor(mOpColor);
    }
    else if(mRuler == 3)
    {
        mLineNode->setColor(mColor.toStdString());
        mLineNode->setWidth(mWidth);
        mLineNode->setShowSlope(true);
        mLineNode->setColor(mOpColor);

    }

}

void LinePropertiesModel::setMeasureHeight(MeasureHeight *measureHeight)
{
    mLineNode = nullptr;
    mMeasureHeight = measureHeight;
    if(!measureHeight){
        return;
    }
    mMeasureHeight->setColor(mColor.toStdString());
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

LineProperties::LineProperties(QQmlEngine *engine,UIHandle *muiHandle, QObject *parent ):
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(muiHandle)
{
    //--show property window---------------------------------------------------------------------------------
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mLineProperties = new LinePropertiesModel();
            mItem->setProperty("lineProperties", QVariant::fromValue<LinePropertiesModel*>(mLineProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/LineProperty.qml"));
    //--------------------------------------------------------------------------------------------------

}

Q_INVOKABLE void LineProperties::show()
{
    mUiHandle->propertiesShow(mItem);
    mLineProperties->setRuler(mIsRuler);
}

void LineProperties::hide()
{

        mUiHandle->propertiesHide(mItem);

}

void LineProperties::setLine(LineNode *line )
{

    mLineProperties->setLine(line);

}

void LineProperties::setMeasureHeight(MeasureHeight *measureHeight)
{
    mLineProperties->setMeasureHeight(measureHeight);
}

void LineProperties::setIsRuler(int value)
{
    mIsRuler = value;
}
