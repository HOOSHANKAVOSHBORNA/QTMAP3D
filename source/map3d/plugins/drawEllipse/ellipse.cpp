#include "ellipse.h"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include "osgEarthAnnotation/AnnotationEditing"

Ellipse::Ellipse(MapItem *mapItem, bool clamp){
    mMapItem = mapItem;
    osgEarth::Symbology::Style ellipseStyle;
    ellipseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() ;
    ellipseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() ;
    ellipseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() ;
    if (clamp){
        ellipseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        ellipseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
    this->setRadiusMajor(osgEarth::Distance(250, osgEarth::Units::MILES));
    this->setRadiusMinor(osgEarth::Distance(100, osgEarth::Units::MILES));
    this->setRotationAngle(osgEarth::Angle(0, osgEarth::Units::DEGREES));
    this->setArcStart(osgEarth::Angle(0, osgEarth::Units::DEGREES));
    this->setArcEnd(osgEarth::Angle(360.0 - 45.0, osgEarth::Units::DEGREES));
    this->setPie(false);
    this->setStyle(ellipseStyle);
}

void Ellipse::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color, 0.5);
    this->setStyle(style);
}

osgEarth::Color Ellipse::getColor(){

    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
}

void Ellipse::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()= clamp;
    this->setStyle(style);
}

osgEarth::Symbology::AltitudeSymbol::Clamping Ellipse::getClamp(){

    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping().get();
}

void Ellipse::setHeight(float height){
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = height;
    if (height<=0){
        style.remove<osgEarth::Symbology::ExtrusionSymbol>();
    }
    this->setStyle(style);
}

float Ellipse::getHeight(){

    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height().get();
}

void Ellipse::setStrokeColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    this->setStyle(style);
}

osgEarth::Color Ellipse::getStrokeColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color();
}

void Ellipse::setStrokeWidth(float Width)
{
     auto style = this->getStyle();
     style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = Width;
     this->setStyle(style);
}

float Ellipse::getStrokeWidth()
{
     auto style = this->getStyle();
     return   style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width().get();
}
