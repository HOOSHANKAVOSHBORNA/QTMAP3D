#include "circle.h"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include "osgEarthAnnotation/AnnotationEditing"


void Circle::setCircleHeight(float hieght)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    this->setStyle(style);

}

Circle::Circle(MapController *mapController, bool clamp)
{
    mMapController = mapController;
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Cyan, 0.5);
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = false;
    if (clamp){
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else {
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

    this->setRadius(osgEarth::Distance(300, osgEarth::Units::KILOMETERS));
    this->setStyle(circleStyle);
    this->setArcStart(osgEarth::Angle(0, osgEarth::Units::DEGREES));
    this->setArcEnd(osgEarth::Angle(360, osgEarth::Units::DEGREES));
    this->setPie(true);









}

void Circle::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color);
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = false;
    this->setStyle(style);
}

void Circle::setClamp(bool clamp)
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
