#include "linenode.h"



LineNode::LineNode(MapController *mapController)
{
    mMapController = mapController;
    mLinePath = new osgEarth::Symbology::Geometry();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style pathStyle;
    //pathStyle.getOrCreate<osgEarth::Symbology::StyleSheet().setScript()
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    pathStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;

    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;

    _options = osgEarth::Features::GeometryCompilerOptions();
    _needsRebuild = true;
    _styleSheet = nullptr;
    _clampDirty = false;
    _index = nullptr;

    _features.push_back( pathFeature );

    osgEarth::Annotation::Style style = pathStyle;
    if (style.empty() && pathFeature->style().isSet())
    {
        style = *pathFeature->style();
    }
    setStyle(style);
    //this->setFeature(pathFeature);
    //    this->setStyle(pathStyle);

}

void LineNode::setLineHeight(float hieght)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    this->setStyle(style);
}

void LineNode::setLineColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = color;
    this->setStyle(style);
}

void LineNode::setLineWidth(float width)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;

    this->setStyle(style);
}

void LineNode::setLineClamp(bool clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    this->setStyle(style);
}

void LineNode::setPointColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = color;
    this->setStyle(style);
}

void LineNode::setPointSize(float size)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = size;
    this->setStyle(style);
}

void LineNode::setPointClamp(bool clamp)
{
    auto style = this->getStyle();
    if (clamp){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;
    this->setStyle(style);
}

osg::ref_ptr<osg::Node> LineNode::makeLineSphere()
{
    osg::Node* sphere = osgDB::readNodeFile("../data/models/sphere.osgb");
    osgEarth::Symbology::Style LiSphereStyle;
    LiSphereStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    LiSphereStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(sphere);
    mCircleModelNode = new osgEarth::Annotation::ModelNode
            (mMapController->getMapNode(),LiSphereStyle);
    //mCircleModelNode->setPosition(geoPos);
}

void LineNode::addPoint(osgEarth::GeoPoint points)
{
    mLinePath->push_back(points.vec3d());
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, osgEarth::SpatialReference::get("wgs84"));
    setFeature(pathFeature);
}

void LineNode::removePoint()
{
    mLinePath->pop_back();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    this->setFeature(pathFeature);
}

void LineNode::clearPath()
{
    mLinePath->clear();
}

int LineNode::getSize()
{
    return static_cast<int>(mLinePath->size());
}
