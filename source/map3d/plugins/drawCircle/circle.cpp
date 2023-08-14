#include "circle.h"
//#include "osgEarth/ModelLayer"
//#include "osgEarth/Layer"
//#include "osgEarthAnnotation/AnnotationEditing"


void Circle::setCircleHeight(float height)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = height;
    if (height<=0){
        style.remove<osgEarth::Symbology::ExtrusionSymbol>();
    }
    this->setStyle(style);

}

float Circle::getCircleHeight()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height().get();
}

Circle::Circle()
{
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() ;
    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = true;
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
//    circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = true;
    this->setStyle(style);
}

void Circle::setLineColor(osgEarth::Color color) {
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    this->setStyle(style);
}

osgEarth::Color Circle::getColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
}

osgEarth::Color Circle::getLineColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color();
}


float Circle::getLineWidth()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width().get();
}




void Circle::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = clamp;
    this->setStyle(style);
}

void Circle::setLineWidth(float width)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;
    this->setStyle(style);
}
