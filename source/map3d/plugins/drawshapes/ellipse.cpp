#include "ellipse.h"

Ellipse::Ellipse(bool clamp)
{
    ellipseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Orange, 0.75);
    if (clamp){
        ellipseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        ellipseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
    this->setRadiusMajor(osgEarth::Distance(250, osgEarth::Units::MILES));
    this->setRadiusMinor(osgEarth::Distance(100, osgEarth::Units::MILES));
    this->setRotationAngle(osgEarth::Angle(0, osgEarth::Units::DEGREES));
    this->setStyle(ellipseStyle);
    this->setArcStart(osgEarth::Angle(0, osgEarth::Units::DEGREES));
    this->setArcEnd(osgEarth::Angle(360.0 - 45.0, osgEarth::Units::DEGREES));
    this->setPie(true);
}

void Ellipse::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color, 0.5);
    this->setStyle(style);
}

void Ellipse::setClamp(bool clamp)
{
    auto style = this->getStyle();
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
    this->setStyle(style);
}

void Ellipse::setHeight(float height)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = height;
    this->setStyle(style);
}
