#include "line.h"

Line::Line()
{
    mLinePath = new osgEarth::Symbology::Geometry();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, osgEarth::SpatialReference::get("wgs84"));
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style pathStyle;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;


    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    mPathNode = new osgEarth::Annotation::FeatureNode(pathFeature, pathStyle);

}

void Line::setLineStyle(osgEarth::Color color, float width, bool clamp)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    mPathNode->setStyle(style);
}

void Line::setPointStyle(osgEarth::Color color, float size, bool clamp)
{
    osgEarth::Symbology::Style style;
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = size;
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = color;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    if (clamp){
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    mPathNode->setStyle(style);
}

void Line::addPoint(const osg::Vec3d &vec)
{
    mLinePath->push_back(vec);
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, osgEarth::SpatialReference::get("wgs84"));
    mPathNode->setFeature(pathFeature);
}

void Line::removePoint()
{
    mLinePath->pop_back();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, osgEarth::SpatialReference::get("wgs84"));
    mPathNode->setFeature(pathFeature);

}

int Line::getSize()
{
    return mLinePath->size();
}

osgEarth::Annotation::FeatureNode *Line::getNode()
{
    return mPathNode;
}
