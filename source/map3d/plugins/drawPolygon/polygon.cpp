#include "polygon.h"

#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthFeatures/GeometryCompiler>
#include "osgEarthAnnotation/AnnotationEditing"
#include "osgEarth/Tessellator"

Polygon::Polygon(MapController *mapController, bool clamp)

{
    mPolygonGeom = new osgEarth::Features::Polygon();
    mMapController = mapController;
    osgEarth::Features::Feature* feature = new osgEarth::Features::Feature(mPolygonGeom, mMapController->getMapSRS());
    feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;
    osgEarth::Symbology::Style geomStyle;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() /*= osgEarth::Color::Purple*/;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() /*= 2.0f*/;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 200;
    geomStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() /*= osg::Vec4f(1,1,1,1)*/;
    geomStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;

    //geomStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 80000;

//    geomStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 100000;

    //geomStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

    if (clamp){
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }

    geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;


    _options = osgEarth::Features::GeometryCompilerOptions();
    _needsRebuild = true;
    _styleSheet = nullptr;
    _clampDirty = true;
    _index = nullptr;

    _features.push_back( feature );

    osgEarth::Annotation::Style style = geomStyle;
    if (style.empty() && feature->style().isSet())
    {
        style = *feature->style();
    }
    setStyle(style);
}

double Polygon::getSize()
{
    return mPolygonGeom->size();
}

osgEarth::Color Polygon::getFillColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
}

osgEarth::Color Polygon::getLineColor()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color();
}

void Polygon::setLineColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    this->setStyle(style);
}


void Polygon::setFillColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = color;
    this->setStyle(style);
}


void Polygon::setLineWidth(float width)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;
    this->setStyle(style);
}

float Polygon::getLineWidth()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width().get();
}

float Polygon::getHeight()
{
    auto style = this->getStyle();
    return style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height().get();
}

void Polygon::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()= clamp;
    this->setStyle(style);
}

void Polygon::addPoints(osgEarth::GeoPoint point)
{
    auto fea = this->getFeature();
    mPolygonGeom->push_back(point.vec3d());
    dirty();
    fea->setGeometry(mPolygonGeom);
}
void Polygon::clearPoints()
{
    mPolygonGeom->clear();
}

void Polygon::removePoint()
{
    mPolygonGeom->pop_back();
    dirty();
}

void Polygon::setHeight(float height)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = height;
    if (height<=0){
        style.remove<osgEarth::Symbology::ExtrusionSymbol>();
    }
    this->setStyle(style);
}
