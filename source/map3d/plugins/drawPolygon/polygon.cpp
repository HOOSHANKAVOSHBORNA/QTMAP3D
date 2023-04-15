#include "polygon.h"

#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthFeatures/GeometryCompiler>
#include "osgEarthAnnotation/AnnotationEditing"

Polygon::Polygon(MapController *mapController, bool clamp)

{
    mPolygonGeom = new osgEarth::Features::Polygon();
    mMapController = mapController;
    osgEarth::Features::Feature* feature = new osgEarth::Features::Feature(mPolygonGeom, mMapController->getMapSRS());
    feature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style geomStyle;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Purple;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0f;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    geomStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osg::Vec4f(1,1,1,1);
    geomStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    //geomStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 0;
//    geomStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

    if (clamp){
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }

    geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;


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
    return mPolygonGeom->getLength();
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

void Polygon::setClamp(bool clamp)
{
    auto style = this->getStyle();
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
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
    this->setStyle(style);
}



