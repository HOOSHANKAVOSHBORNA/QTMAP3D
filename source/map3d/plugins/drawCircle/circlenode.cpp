#include "circlenode.h"



void CircleNode::setCircleHeight(float hieght)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    this->setStyle(style);

}

CircleNode::CircleNode(MapController *mapController, bool clamp)
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

void CircleNode::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(color);
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = false;
    this->setStyle(style);
}

osgEarth::Color CircleNode::getColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
}



void CircleNode::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = clamp;
    this->setStyle(style);
}
