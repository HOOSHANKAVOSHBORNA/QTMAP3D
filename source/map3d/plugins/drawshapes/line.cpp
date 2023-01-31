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
                = mColor;
        pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mWidth;

    }
    else {
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mColor;
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
        pathStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

    }
    if (mClamp){
        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
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
    mColor = color;
    if (!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()
            ->stroke()->color()= color;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()
                ->fill()->color()= color;
    }
    mPathNode->setStyle(style);
}

void Line::setWidth(float width)
{
    auto style = mPathNode->getStyle();
    mWidth = width;
    if(!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()
            ->width() = width;
}
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()
                ->size() = width;
    }
    mPathNode->setStyle(style);
}

void Line::setClamp(bool clamp)
{
    mClamp = clamp;
    auto style = mPathNode->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique()
            = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    mPathNode->setStyle(style);
}

void Line::switchLP(bool lineMode)
{
    if (lineMode){
        osgEarth::Symbology::Style lineStyle;
        lineStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
        lineStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0;
        lineStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
        lineStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
        if(mClamp){
            lineStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        }
        else {
            lineStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                    = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
        }
        lineStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        mPathNode->setStyle(lineStyle);
    }
    else {
        osgEarth::Symbology::Style pointStyle;
        pointStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color()
                = osgEarth::Color::Red;

        pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique()
                = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        pointStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = 50;
        if(mClamp){
            pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        }
        else {
            pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                    = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
        }
        pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        mPathNode->setStyle(pointStyle);
    }
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
