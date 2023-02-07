#include "linenode.h"
#include "drawshapeautoscaler.h"


LineNode::LineNode(MapController *mapController, bool point)
{
    sphere = osgDB::readNodeFile("../data/models/sphere.osgb");
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
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique()
            = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;


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

    mCircleGr = new osg::Group;
    addChild(mCircleGr);

}

void LineNode::setLineHeight(float hieght)
{
    auto style = this->getStyle();
    if (!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = hieght;
    }
    this->setStyle(style);
}

void LineNode::setColor(osgEarth::Color color)
{
    auto style = this->getStyle();
    mColor = color;
    if (!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()
            ->stroke()->color()= mColor;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()
                ->fill()->color()= mColor;
    }
    this->setStyle(style);
}

void LineNode::setWidth(float width)
{
    auto style = this->getStyle();
    mWidth = width;
    if(!mIsPoint){
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()
            ->width() = mWidth;
}
    else{
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()
                ->size() = mWidth;
    }
    this->setStyle(style);
}

void LineNode::setClamp(bool clamp)
{
    mClamp = clamp;
    auto style = this->getStyle();
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
    this->setStyle(style);
}

void LineNode::dashLine(bool dashLine)
{
    if (!dashLine){
        osgEarth::Symbology::Style lineStyle;
        lineStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mColor;
        lineStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
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
        this->setStyle(lineStyle);
    }
    else {
        osgEarth::Symbology::Style pointStyle;
        pointStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color()
                = mColor;

        pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique()
                = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        pointStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mWidth;
        pointStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
        pointStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 30000;
        if(mClamp){
            pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        }
        else {
            pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()
                    = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
        }
        pointStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        this->setStyle(pointStyle);
    }
}

void LineNode::addPoint(osgEarth::GeoPoint points)
{
    mLinePath->push_back(points.vec3d());
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, points.getSRS());
    setFeature(pathFeature);
    osg::ref_ptr<osg::Material> sphereMat = new osg::Material;
    sphereMat->setDiffuse (osg::Material::FRONT_AND_BACK, osgEarth::Color::DarkGray);
    osgEarth::Symbology::Style LiSphereStyle;
    LiSphereStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(sphere);
    getOrCreateStateSet()->setAttributeAndModes(sphereMat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    mCircleModelNode = new osgEarth::Annotation::ModelNode
            (mMapController->getMapNode(),LiSphereStyle);

    mCircleModelNode->setCullingActive(false);
    mCircleModelNode->addCullCallback(new DrawShapeAutoScaler(1, 0.00001, 3000000));

    mCircleModelNode->setPosition(points);

    mCircleGr->addChild(mCircleModelNode);
    addChild(mCircleGr);
}

void LineNode::removePoint()
{
    mLinePath->pop_back();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    this->setFeature(pathFeature);
    mCircleGr->removeChild(mCircleModelNode);
}

void LineNode::clearPath()
{
    mLinePath->clear();
}

int LineNode::getSize()
{
    return static_cast<int>(mLinePath->size());
}

void LineNode::pointVisibilty(bool visibility)
{
        mCircleGr->setNodeMask(visibility);
}
