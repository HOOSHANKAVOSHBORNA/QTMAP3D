#include "rect.h"

Rect::Rect(bool clamp, double width, double height)
{
    rectStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Red, 0.5);
     if (clamp){
         rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
     }
     else {
         rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
     }
    rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    this->setWidth(osgEarth::Distance(width, osgEarth::Units::KILOMETERS));
    this->setHeight(osgEarth::Distance(height, osgEarth::Units::KILOMETERS));
    this->setStyle(rectStyle);
    this->setDynamic(true);


}

void Rect::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color, 0.5);
    this->setStyle(style);
}

void Rect::setClamp(bool clamp)
{
    auto style = this->getStyle();
    if (clamp){
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else {
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
    this->setStyle(style);
}

