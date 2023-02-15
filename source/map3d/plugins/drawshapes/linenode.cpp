#include "linenode.h"
#include "drawshapeautoscaler.h"


LineNode::LineNode(MapController *mapController, bool point)
{
//    sphereMat = new osg::Material;
//    sphere = osgDB::readNodeFile("../data/models/sphere.osgb");
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
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 0;
//    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 1;
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
    mSphereNode = new SphereNode();
    //addChild(mCircleGr);

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

void LineNode::setDashLine(bool dashLine)
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

void LineNode::addPoint(osgEarth::GeoPoint point)
{
    mLinePath->push_back(point.vec3d());
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, point.getSRS());
    setFeature(pathFeature);
    if(mIsPointVisible)
    {
//        osg::ref_ptr<osg::Material> sphereMat = new osg::Material;
//        sphereMat->setDiffuse (osg::Material::FRONT_AND_BACK, pointColor);
//        osgEarth::Symbology::Style LiSphereStyle;
//        LiSphereStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(sphere);
//        mCircleGr->getOrCreateStateSet()->setAttributeAndModes(sphereMat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
//        mCircleModelNode = new osgEarth::Annotation::ModelNode
//                (mMapController->getMapNode(),LiSphereStyle);

//        mCircleModelNode->setCullingActive(false);
//        mCircleModelNode->addCullCallback(new DrawShapeAutoScaler(1, 0.00001, 3000000));

//        mCircleModelNode->setPosition(points);
        mSphereNode = new SphereNode();
        mSphereNode->setCullingActive(false);
        mSphereNode->addCullCallback(new DrawShapeAutoScaler(1, 0.00001, 3000000));

        mSphereNode->setPosition(point);
        mSphereNode->setRadius(osgEarth::Distance(mWidth, osgEarth::Units::METERS));
        mSphereNode->setColor(mPointColor);
        mCircleGr->addChild(mSphereNode);
        addChild(mCircleGr);
    }
}

void LineNode::removePoint()
{
    mLinePath->pop_back();
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    this->setFeature(pathFeature);
    mCircleGr->removeChild(mSphereNode);
    addChild(mCircleGr);
}

void LineNode::removeFirstPoint()
{
    mLinePath->erase(mLinePath->begin());
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(mLinePath, mMapController->getMapSRS());
    this->setFeature(pathFeature);
//    unsigned index = mCircleGr->getNumChildren()-1;
    mCircleGr->removeChildren(0, 1);
    addChild(mCircleGr);
}

void LineNode::clearPath()
{
    mLinePath->clear();
}

int LineNode::getSize()
{
    return static_cast<int>(mLinePath->size());
}

void LineNode::setPointVisibilty(bool visibility)
{
    mIsPointVisible = visibility;
    mCircleGr->setNodeMask(visibility);
}

void LineNode::setPointColor(osgEarth::Color color)
{
    mPointColor = color;
    auto material = new osg::Material;
    material->setDiffuse (osg::Material::FRONT_AND_BACK, color);
    mCircleGr->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
}
