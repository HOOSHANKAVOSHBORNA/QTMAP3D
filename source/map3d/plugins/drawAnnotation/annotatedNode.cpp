#include "annotatedNode.h"
#include <osgEarth/GeoMath>
#include <osgEarthAnnotation/LabelNode>
#include <osg/Point>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QDialog>
#include <osgEarthSymbology/Geometry>
#include <osgEarth/ElevationQuery>


AnnotatedNode::AnnotatedNode(MapItem *mapItem, GeneralType type)
{
    mMapItem = mapItem;
    mType =  type;
    if(mType == GeneralType::POLYLINETYPE){
        auto geometry = new osgEarth::Symbology::Geometry();
        mPathFeature = new osgEarth::Features::Feature(geometry, mMapItem->getMapSRS());
    }

    else{
        osg::ref_ptr<osgEarth::Features::Polygon> geometry = new osgEarth::Features::Polygon();
        mPathFeature = new osgEarth::Features::Feature(geometry, mMapItem->getMapSRS());
    }

    mPathFeature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

    osgEarth::Symbology::Style pathStyle;
    //    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
    //    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
    //    pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;

    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mColor;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->lineCap() = osgEarth::Symbology::Stroke::LINECAP_ROUND;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = mTessellation;

    _options = osgEarth::Features::GeometryCompilerOptions();
    _needsRebuild = true;
    _styleSheet = nullptr;
    _clampDirty = false;
    _index = nullptr;

    setFeature(mPathFeature);
    setStyle(pathStyle);
    mLabelGroup = new osg::Group;
    addChild(mLabelGroup);
    mLabelGroup->setNodeMask(false);
}

AnnotatedNode::~AnnotatedNode()
{

}

void AnnotatedNode::create(std::vector<osg::Vec3d> *points)
{
    if (mType == GeneralType::POLYLINETYPE){
        auto geometry= osgEarth::Features::Geometry::create(osgEarth::Features::Geometry::TYPE_LINESTRING, points);
        getFeature()->setGeometry(geometry);
    }
    else{
         auto geometry= osgEarth::Features::Geometry::create(osgEarth::Features::Geometry::TYPE_POLYGON, points);
        getFeature()->setGeometry(geometry);
    }
    dirty();
}

void AnnotatedNode::addPoint(osgEarth::GeoPoint point)
{
    auto geometry = getFeature()->getGeometry();
    geometry->push_back(point.vec3d());
    dirty();

    if(getSize() >= 2 /*&& type == GeneralType::POLYLINE*/)
    {
        osgEarth::GeoPoint p1(mMapItem->getMapSRS() ,geometry->at(geometry->size() - 2));
        osgEarth::GeoPoint p2(mMapItem->getMapSRS(),geometry->at(geometry->size() - 1));

        std::vector<osg::Vec3d> distanceVectorPoint;
        distanceVectorPoint.push_back(p1.vec3d());
        distanceVectorPoint.push_back(p2.vec3d());
        double distance = osgEarth::GeoMath().rhumbDistance(distanceVectorPoint);

        double altitude = p2.z() - p1.z();

        double bearingRadian = osgEarth::GeoMath().bearing(osg::DegreesToRadians(p1.y()),
                                                           osg::DegreesToRadians(p1.x()),
                                                           osg::DegreesToRadians(p2.y()),
                                                           osg::DegreesToRadians(p2.x()));
        double bearing = osg::RadiansToDegrees(bearingRadian);
        if (bearing < 0){
            bearing += 360;
        }

        double slope = (geometry->at(geometry->size() - 1).z() - geometry->at(geometry->size() - 2).z()) / p1.distanceTo(p2);
        slope = std::asin(slope);
        slope = osg::RadiansToDegrees(slope);

        //        osg::ref_ptr<osg::Image> image = new osg::Image;
        osg::ref_ptr<StatusNode> statusNode = new StatusNode(mMapItem);
        LabelData data;
        data.distance = distance;
        data.altitude = altitude;
        data.bearing = bearing;
        data.slope = slope;
        data.statusNode = statusNode;

        updateStatusNode(data);
        //        data.image = image;
        //        data.qImage = qImage;
        mVecLabelData.push_back(data);
        //        statusNode->setIconImage(image);



        double latMidP;
        double lonMidP;
        double zMidP = (p1.z() + p2.z())/2;
        //		osgEarth::GeoPoint midPoint(mMapController->getMapSRS(),
        //									(mLineGeometry->at(mLineGeometry->size() - 2) + mLineGeometry->at(mLineGeometry->size() -1 )) / 2);

        osgEarth::GeoMath().midpoint(osg::DegreesToRadians(p1.x()),
                                     osg::DegreesToRadians(p1.y()),
                                     osg::DegreesToRadians(p2.x()),
                                     osg::DegreesToRadians(p2.y()),
                                     latMidP, lonMidP);

        osgEarth::GeoPoint midpoint(mMapItem->getMapSRS()->getGeographicSRS(),osg::RadiansToDegrees(latMidP),osg::RadiansToDegrees(lonMidP),zMidP);
        statusNode->setPosition(midpoint);


        //        statusNode->setStyle(placeNode->getStyle());
        mLabelGroup->addChild(statusNode);
    }
    addChild(mLabelGroup);


}

osgEarth::GeoPoint AnnotatedNode::getPoint(int index)
{
    return osgEarth::GeoPoint(mMapItem->getMapSRS() ,getFeature()->getGeometry()->at(index));
}

void AnnotatedNode::removePoint()
{
    getFeature()->getGeometry()->pop_back();
    dirty();
    //    auto qImage = mVecLabelData[mVecLabelData.size()-1 ].qImage;
    //    if(qImage)
    //        delete  qImage;
        mVecLabelData.pop_back();
        mLabelGroup->removeChild(getSize()-1);
        addChild(mLabelGroup);
}

void AnnotatedNode::removeFirstPoint()
{
    getFeature()->getGeometry()->erase(getFeature()->getGeometry()->begin());
    //    auto qImage = mVecLabelData[0].qImage;
    //    if(qImage)
    //        delete  qImage;
    mVecLabelData.erase(mVecLabelData.begin());
    dirty();
    mLabelGroup->removeChild(0, 1);

    addChild(mLabelGroup);
}

void AnnotatedNode::clear()
{

    getFeature()->getGeometry()->clear();
    dirty();
    //    for(const auto& labelData: mVecLabelData){
    //        if(labelData.qImage)
    //            delete labelData.qImage;
    //    }
    mVecLabelData.clear();
    mLabelGroup->removeChildren(0,mLabelGroup->getNumChildren());
    addChild(mLabelGroup);

}

int AnnotatedNode::getSize()
{
    return static_cast<int>(getFeature()->getGeometry()->size());
}

osgEarth::Color AnnotatedNode::getFillColor() const
{
    return mColor;
}

void AnnotatedNode::setFillColor(const osgEarth::Color &color)
{
    if(mColor == color)
        return;
    mColor = color;
    auto style = getStyle();
    if (mType == GeneralType::POLYLINETYPE){
        style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color()= mColor;
    }
    else
        style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = mColor;

    setStyle(style);
    addChild(mLabelGroup);
}

float AnnotatedNode::getWidth() const
{
    return mWidth;
}

void AnnotatedNode::setWidth(float width)
{
    mWidth = width;
    auto style = getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = mWidth;
    setStyle(style);
    addChild(mLabelGroup);
}

float AnnotatedNode::getHeight() const
{
    return mHeight;
}

void AnnotatedNode::setHeight(float height)
{
    mHeight = height;
    auto style = getStyle();
    style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = mHeight;
    if (height<=0){
        style.remove<osgEarth::Symbology::ExtrusionSymbol>();
    }
    setStyle(style);
    addChild(mLabelGroup);
}

osgEarth::Symbology::AltitudeSymbol::Clamping AnnotatedNode::getClamp() const
{
    return mClamp;
}

void AnnotatedNode::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp)
{
    if(mClamp == clamp)
        return;

    mClamp = clamp;
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = clamp;
    setStyle(style);
    addChild(mLabelGroup);
}

unsigned AnnotatedNode::getTessellation() const
{
    return mTessellation;
}

void AnnotatedNode::setTessellation(const unsigned &tessellation)
{
    if(mTessellation == tessellation)
        return;
    mTessellation = tessellation;
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = tessellation;
    setStyle(style);
    addChild(mLabelGroup);
}

//--------------------------------------------------------------
bool AnnotatedNode::getPointVisible() const
{
    return mPointVisible;
}

void AnnotatedNode::setPointVisible(bool value)
{
    if(mPointVisible == value)
        return;

    mPointVisible = value;
    auto style = getStyle();
    if(mPointVisible)
    {
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;
    }
    else
        style.remove<osgEarth::Symbology::PointSymbol>();
    setStyle(style);
    addChild(mLabelGroup);
}

bool AnnotatedNode::getShowBearing() const
{
    return mShowBearing;
}

void AnnotatedNode::setShowBearing(const bool &bearing)
{
    if (mShowBearing == bearing)
        return;

    if(bearing)
        mCount++;
    else if(mCount > 0)
        mCount--;

    mShowBearing = bearing;

    if (mCount > 0){
        mLabelGroup->setNodeMask(true);
        for(auto& data:mVecLabelData)
        {
            updateStatusNode(data);
        }
    }
    else
        mLabelGroup->setNodeMask(false);
}

void AnnotatedNode::setShowDistance(const bool &show)
{
    if (mShowDistance == show)
        return;

    if(show)
        mCount++;
    else if(mCount > 0)
        mCount--;

    mShowDistance = show;

    if (mCount > 0){
        mLabelGroup->setNodeMask(true);
        for(auto& data:mVecLabelData)
        {
            updateStatusNode(data);
        }
    }
    else
        mLabelGroup->setNodeMask(false);

}

bool AnnotatedNode::getShowDistance() const
{
    return  mShowDistance;
}

bool AnnotatedNode::getShowSlope() const
{
    return mShowSlope;
}

void AnnotatedNode::setShowSlope(bool showSlope)
{

    if (mShowSlope == showSlope)
        return;

    if(showSlope)
        mCount++;
    else if(mCount > 0)
        mCount--;

    mShowSlope = showSlope;

    if (mCount > 0){
        mLabelGroup->setNodeMask(true);
        for(auto& data:mVecLabelData)
        {
            updateStatusNode(data);
        }
    }
    else
        mLabelGroup->setNodeMask(false);
}

bool AnnotatedNode::getShowAltitude() const
{
    return mShowAltitude;
}

void AnnotatedNode::setShowAltitude(bool showAltitude)
{
    if (mShowAltitude == showAltitude)
        return;

    if(showAltitude)
        mCount++;
    else if(mCount > 0)
        mCount--;

    mShowAltitude = showAltitude;

    if (mCount > 0){
        mLabelGroup->setNodeMask(true);
        for(auto& data:mVecLabelData)
        {
            updateStatusNode(data);
        }
    }
    else
        mLabelGroup->setNodeMask(false);
}

osgEarth::Color AnnotatedNode::getPointColor() const
{
    return mPointColor;
}

void AnnotatedNode::setPointColor(const osgEarth::Color &pointColor)
{
    if(mPointColor == pointColor)
        return;
    mPointColor = pointColor;
    if(mPointVisible)
    {
        auto style = getStyle();
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = mPointColor;
        setStyle(style);
        addChild(mLabelGroup);
    }
}

float AnnotatedNode::getPointWidth() const
{
    return mPointWidth;
}

void AnnotatedNode::setPointWidth(float pointWidth)
{
    mPointWidth = pointWidth;
    auto style = getStyle();
    if(mPointVisible)
    {
        style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = mPointWidth;
        setStyle(style);
        addChild(mLabelGroup);
    }
}

bool AnnotatedNode::getSmooth() const
{
    return mSmooth;
}

void AnnotatedNode::setSmooth(bool smooth)
{
    mSmooth = smooth;
    auto sStyle = getStyle();
    if(mPointVisible)
    {
        sStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->smooth() = mSmooth;
        setStyle(sStyle);
        addChild(mLabelGroup);

    }
}
//---------------------------------------------------------------
void AnnotatedNode::updateStatusNode( LabelData &lableData)
{
    std::vector<NodeFieldData> dataList;

    if (mShowDistance){
        NodeFieldData data;
        QString strVal;
        data.name = "d";
        if (lableData.distance >= 1000)
            strVal = QObject::tr("%1 km").arg(lableData.distance/1000,0,'f',2);
        else
            strVal = QObject::tr("%1 m").arg(lableData.distance,0,'f',2);
        data.value = strVal;
        dataList.push_back(data);
    }
    if (mShowAltitude){
        NodeFieldData data;
        QString strVal;
        data.name = "h";
        if (lableData.altitude >= 1000)
            strVal = QObject::tr("%1 km").arg(lableData.altitude/1000, 0, 'f', 2);
        else
            strVal = QObject::tr("%1 m").arg(lableData.altitude,0,'f',2);
        data.value = strVal;
        dataList.push_back(data);
    }
    if (mShowBearing){
        NodeFieldData data;
        QString strVal = QString::number(lableData.bearing, 'f', 2);
        data.name = "b";
        data.value = strVal;
        dataList.push_back(data);
    }

    if (mShowSlope){
        NodeFieldData data;
        QString strVal = QString::number(lableData.slope, 'f', 2);
        data.name = "s";
        data.value = strVal;
        dataList.push_back(data);
    }
    lableData.statusNode->setFieldData(QString::fromStdString(getName()), dataList);

}

AnnotatedNode::GeneralType AnnotatedNode::type() const
{
    return mType;
}

osgEarth::Color AnnotatedNode::strokeColor() const
{
    return mStrokeColor;
}

void AnnotatedNode::setStrokeColor(const osgEarth::Color &newStrokeColor)
{
    mStrokeColor = newStrokeColor;
    auto style = getStyle();
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mStrokeColor;
        setStyle(style);
        addChild(mLabelGroup);
}

float AnnotatedNode::strokeWidth() const
{
    return mStrokeWidth;
}

void AnnotatedNode::setStrokeWidth(float width)
{
    if(mStrokeWidth != width){
        mStrokeWidth = width;
        auto style = this->getStyle();
        style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = width;
        this->setStyle(style);
    }
}

PolyLineData *AnnotatedNode::polyLineData() const
{
    return mPolyLineData;
}

void AnnotatedNode::setPolyLineData(PolyLineData *newPolyLineData)
{
    mPolyLineData = newPolyLineData;
}

PolygonData *AnnotatedNode::polygonData() const
{
    return mPolygonData;
}

void AnnotatedNode::setPolygonData(PolygonData *newPolygonData)
{
    mPolygonData = newPolygonData;
}



