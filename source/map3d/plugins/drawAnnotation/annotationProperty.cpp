#include "annotationProperty.h"
#include <QtDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <utility.h>
#include "property.h"


AnnotationProperty::AnnotationProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus(false);
    setStrokeStatus(true);
    setPointsStatus(true);
    setTesselationStatus(true);
    setClampStatus(true);
    setHeightStatus(false);
    setHeight(0);
}


void AnnotationProperty::setStrokeWidth(double width)
{

    Property::setStrokeWidth(width);
    if(mAnnotatedNode)
        mAnnotatedNode->setWidth(width);
}

void AnnotationProperty::setStrokeColor(const QColor &color)
{
    Property::setStrokeColor(color);
    if(mAnnotatedNode)
//        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(color));
        mAnnotatedNode->setStrokeColor(Utility::qColor2osgEarthColor(color));

}

void AnnotationProperty::setFillColor(const QColor &color)
{
    Property::setFillColor(color);
    if(mAnnotatedNode)
        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(color));
}

void AnnotationProperty::setPointsVisible(bool visible)
{
    Property::setPointsVisible(visible);
    if(mAnnotatedNode)
        mAnnotatedNode->setPointVisible(visible);
}

void AnnotationProperty::setPointsColor(const QColor &point)
{
    Property::setPointsColor(point);
    if(mAnnotatedNode)
        mAnnotatedNode->setPointColor(Utility::qColor2osgEarthColor(point));

}

void AnnotationProperty::setPointsWidth(double point)
{
    Property::setPointsWidth(point);
    if(mAnnotatedNode)
        mAnnotatedNode->setPointWidth(point);
}

void AnnotationProperty::setPointsSmooth(bool point)
{
    Property::setPointsSmooth(point);
    if(mAnnotatedNode)
        mAnnotatedNode->setSmooth(point);
}

void AnnotationProperty::setShowBearing(bool showBearing)
{
    Property::setShowBearing(showBearing);
    if(mAnnotatedNode)
        mAnnotatedNode->setShowBearing(showBearing);
}

void AnnotationProperty::setShowSlop(bool slop)
{
    Property::setShowSlop(slop);
    if(mAnnotatedNode)
        mAnnotatedNode->setShowSlope(slop);
}

void AnnotationProperty::setTesselation(double tesselation)
{
    Property::setTesselation(tesselation);
    if(mAnnotatedNode)
        mAnnotatedNode->setTessellation(tesselation);
}

void AnnotationProperty::setClamp(int clamp)
{
    Property::setClamp(clamp);
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
    mAnnotatedNode->setClamp(clampEnum);
}

void AnnotationProperty::setShowLen(bool showLen)
{
    Property::setShowLen(showLen);
    if(mAnnotatedNode)
        mAnnotatedNode->setShowDistance(showLen);
}

void AnnotationProperty::setHeight(double height)
{
    Property::setHeight(height);
    if(mAnnotatedNode)
        mAnnotatedNode->setHeight(height);
}

void AnnotationProperty::setShowAltitude(bool showAltitude)
{
    Property::setShowAltitude(showAltitude);
    if(mAnnotatedNode)
        mAnnotatedNode->setShowAltitude(showAltitude);
}

AnnotatedNode *AnnotationProperty::getAnnotatedNode() const
{
    return mAnnotatedNode;
}

void AnnotationProperty::setAnnotatedNode(AnnotatedNode *newLine)
{
    mAnnotatedNode = newLine;
    if(mAnnotatedNode){
        mAnnotatedNode->setWidth(getStrokeWidth());
        if(mAnnotatedNode->type() == AnnotatedNode::GeneralType::POLYGONTYPE){
            setFillColorStatus(true);
            setHeightStatus(true);
        }
        mAnnotatedNode->setStrokeColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mAnnotatedNode->setPointColor(Utility::qColor2osgEarthColor(getPointsColor()));
        mAnnotatedNode->setPointWidth(getPointsWidth());
        mAnnotatedNode->setSmooth(getPointsSmooth());
        mAnnotatedNode->setHeight(getHeight());
        mAnnotatedNode->setShowBearing(getShowBearing());
        mAnnotatedNode->setShowSlope(getShowSlop());
        mAnnotatedNode->setTessellation(getTesselation());
        mAnnotatedNode->setShowDistance(getShowLen());
        mAnnotatedNode->setShowAltitude(true);
        mAnnotatedNode->setPointVisible(getPointsVisible());

        setName(QString::fromStdString(mAnnotatedNode->getName()));
    }
}

void AnnotationProperty::setRuler(AnnotatedNode *newLine)
{
    setShowLenStatus(false);
    setBearingStatus(false);
    setShowSlopStatus(false);
    setClampStatus(false);

    mAnnotatedNode = newLine;
    if(mAnnotatedNode){
        mAnnotatedNode->setWidth(getStrokeWidth());
        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mAnnotatedNode->setPointColor(Utility::qColor2osgEarthColor(getPointsColor()));
        mAnnotatedNode->setPointWidth(getPointsWidth());
        mAnnotatedNode->setSmooth(getPointsSmooth());
        mAnnotatedNode->setShowBearing(false);
        mAnnotatedNode->setShowSlope(false);
        mAnnotatedNode->setTessellation(getTesselation());
        mAnnotatedNode->setShowDistance(getShowLen());
        mAnnotatedNode->setPointVisible(getPointsVisible());

        setName(QString::fromStdString(mAnnotatedNode->getName()));
    }
}

void AnnotationProperty::setMeasureHeight(AnnotatedNode *newLine)
{
    setShowLenStatus(false);
    setBearingStatus(false);
    setShowSlopStatus(false);
    setClampStatus(false);

    mAnnotatedNode = newLine;
    if(mAnnotatedNode){
        mAnnotatedNode->setWidth(getStrokeWidth());
        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mAnnotatedNode->setPointColor(Utility::qColor2osgEarthColor(getPointsColor()));
        mAnnotatedNode->setPointWidth(getPointsWidth());
        mAnnotatedNode->setSmooth(getPointsSmooth());
        mAnnotatedNode->setShowBearing(false);
        mAnnotatedNode->setShowSlope(false);
        mAnnotatedNode->setTessellation(getTesselation());
        mAnnotatedNode->setShowDistance(false);
        mAnnotatedNode->setShowAltitude(true);
        mAnnotatedNode->setPointVisible(getPointsVisible());

        setName(QString::fromStdString(mAnnotatedNode->getName()));
    }
}

void AnnotationProperty::setMesureSlope(AnnotatedNode *newLine)
{
    setShowLenStatus(false);
    setBearingStatus(false);
    setShowSlopStatus(false);
    setClampStatus(false);

    mAnnotatedNode = newLine;
    if(mAnnotatedNode){
        mAnnotatedNode->setWidth(getStrokeWidth());
        mAnnotatedNode->setFillColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mAnnotatedNode->setPointColor(Utility::qColor2osgEarthColor(getPointsColor()));
        mAnnotatedNode->setPointWidth(getPointsWidth());
        mAnnotatedNode->setSmooth(getPointsSmooth());
        mAnnotatedNode->setShowBearing(false);
        mAnnotatedNode->setShowSlope(getShowSlop());
        mAnnotatedNode->setTessellation(getTesselation());
        mAnnotatedNode->setShowDistance(false);
        mAnnotatedNode->setPointVisible(getPointsVisible());

        setName(QString::fromStdString(mAnnotatedNode->getName()));
    }
}


