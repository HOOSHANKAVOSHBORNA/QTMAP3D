#include "smallmap.h"
#include <osgViewer/config/SphericalDisplay>
SmallMap::SmallMap(QQuickItem *parent):
    MapItem(parent)
{
    initializeOsgEarth();
}

void SmallMap::initializeOsgEarth()
{
    qDebug() << "{{{{{{{";
    mLocation = new osgEarth::Annotation::RectangleNode();
    osgEarth::Symbology::Style rectseStyle;
    rectseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color(osg::Vec4f(1.0, 1.0, 1.0, 1.0));
    rectseStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width()  = 50;
    rectseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::Clamping::CLAMP_TO_TERRAIN;
    rectseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
//    rectseStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    //    this->setWidth(osgEarth::Distance(10, osgEarth::Units::KILOMETERS));
    //    this->setHeight(osgEarth::Distance(5, osgEarth::Units::KILOMETERS));

    mLocation->setStyle(rectseStyle);
    mLocation->setWidth(500000);
    mLocation->setHeight(500000);
    getMapNode()->addChild(mLocation);

//    mLocation->setPosition(
//    mLocation->addChild(boxShape);

}

void SmallMap::setLocation(const osgEarth::GeoPoint &geoPos)
{
    mLocation->setPosition(geoPos);
    qDebug() << geoPos.toString();
}
