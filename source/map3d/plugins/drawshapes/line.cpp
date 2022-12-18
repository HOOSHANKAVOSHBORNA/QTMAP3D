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
    pathStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    mPathNode = new osgEarth::Annotation::FeatureNode(pathFeature, pathStyle);

}

void Line::setLineHeight(float hieght)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    mPathNode->setStyle(style);
}


void Line::setLineColor(osgEarth::Color color)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    mPathNode->setStyle(style);
}

void Line::setLineWidth(float width)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;

    mPathNode->setStyle(style);
}

void Line::setLineClamp(bool clamp)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    if (clamp){
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    mPathNode->setStyle(style);
}

void Line::setPointColor(osgEarth::Color color)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = color;
    mPathNode->setStyle(style);
}

void Line::setPointSize(float size)
{
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = size;
    mPathNode->setStyle(style);
}

void Line::setPointClamp(bool clamp)
{
    auto style = mPathNode->getStyle();
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
    return static_cast<int>(mLinePath->size());
}

osgEarth::Annotation::FeatureNode *Line::getNode()
{
    return mPathNode;
}
