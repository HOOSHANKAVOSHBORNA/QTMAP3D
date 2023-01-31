#include "line.h"

Line::Line(MapController *mapController, bool point)
{
    mIsPoint = point;
    mMapController = mapController;
    mLinePath = new osgEarth::Symbology::Geometry();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style pathStyle;
    if (point){
        pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color()
                = osgEarth::Color::Red;

        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique()
                = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = 50;
        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    }
    else {
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0;
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
        pathStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    }
    //pathStyle.getOrCreate<osgEarth::Symbology::StyleSheet().setScript()

    mPathNode = new osgEarth::Annotation::FeatureNode(pathFeature, pathStyle);

}

void Line::setLineHeight(float hieght)
{
    auto style = mPathNode->getStyle();
    if (!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    }
    mPathNode->setStyle(style);
}


void Line::setColor(osgEarth::Color color)
{
    auto style = mPathNode->getStyle();
    if (!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color()
            = color;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color()
                = color;
    }
    mPathNode->setStyle(style);
}

void Line::setWidth(float width)
{
    auto style = mPathNode->getStyle();
    if(!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;
}
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = width;
    }
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

void Line::clearPoints()
{
    mLinePath->clear();
}

int Line::getSize()
{
    return static_cast<int>(mLinePath->size());
}

osgEarth::Annotation::FeatureNode *Line::getNode()
{
    return mPathNode;
}
