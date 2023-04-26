#include "rect.h"

#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include "osgEarthAnnotation/AnnotationEditing"

Rect::Rect(MapController *mapController, bool clamp){
    mMapController = mapController;
    osgEarth::Symbology::Style rectseStyle;
    rectseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() ;
    rectseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() ;
    rectseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() ;
    if (clamp){
        rectseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        rectseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
    this->setWidth(osgEarth::Distance(10, osgEarth::Units::KILOMETERS));
    this->setHeight(osgEarth::Distance(5, osgEarth::Units::KILOMETERS));

    this->setStyle(rectseStyle);
}

void Rect::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color, 0.5);
    this->setStyle(style);
}

osgEarth::Color Rect::getColor(){

    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
}

void Rect::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()= clamp;
    this->setStyle(style);
}

osgEarth::Symbology::AltitudeSymbol::Clamping Rect::getClamp(){

    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping().get();
}

void Rect::setStrokeColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
}

osgEarth::Color Rect::getStrokeColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color();
}

void Rect::setStrokeWidth(float Width)
{
     auto style = this->getStyle();
     style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = Width;
}

float Rect::getStrokeWidth()
{
     auto style = this->getStyle();
     return   style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width().get();
}

