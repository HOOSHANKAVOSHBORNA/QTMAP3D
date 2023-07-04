#include "polygon.h"

#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthFeatures/GeometryCompiler>
#include "osgEarthAnnotation/AnnotationEditing"
#include "qpainter.h"

Polygon::Polygon(MapItem *mapItem)
{
    mPolygonGeom = new osgEarth::Features::Polygon();
    mMapItem = mapItem;
    osgEarth::Features::Feature* feature = new osgEarth::Features::Feature(mPolygonGeom, mMapItem->getMapSRS());
    feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;
    osgEarth::Symbology::Style geomStyle;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() /*= osgEarth::Color::Purple*/;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() /*= 2.0f*/;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    //geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 10;
    geomStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() /*= osg::Vec4f(1,1,1,1)*/;
    geomStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;

    //geomStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 800000;

//    geomStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 100000;

    //geomStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;


    geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

    //mLabelGroup = new osg::Group;
    //addChild(mLabelGroup);
    //mLabelGroup->setNodeMask(false);
    addChild(mPlaceNode);

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

Polygon::~Polygon()
{
//    for(auto labelData: mVecLabelData){
//        if(labelData.qImage)
//            delete labelData.qImage;
//    }
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

double Polygon::CalculateAreaOfPolygon()
{
    double totalArea = 0.0;
    double numPoints = getSize();
    qDebug()<<numPoints;
    if (getSize()>2){
    for (int i = 0; i < numPoints; i++)
    {
        osg::Vec3d p1 = mPolygonGeom->at(i);
        osg::Vec3d p2 = mPolygonGeom->at(fmod((i + 1) , numPoints));

        //use Shoelace formula to calculate area of polygon
        totalArea += (p1.x() * p2.y()) - (p2.x() * p1.y());

//        double lat1 = osg::DegreesToRadians(p1.x());
//        double lon1 = osg::DegreesToRadians(p1.y());
//        double lat2 = osg::DegreesToRadians(p2.x());
//        double lon2 = osg::DegreesToRadians(p2.y());



//        double dlon = lon2 - lon1;
//        double dlat = lat2 - lat1;

//        //double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
//        double a = pow(sin(dlat / 2), 2) +
//            pow(sin(dlon / 2), 2) *
//                cos(lat1) * cos(lat2);
//        double c = 2 * asin(sqrt(a));
//        //double c = 2 * atan2(sqrt(a), sqrt(1 - a));
//        double distance = 6371000 * c;
//        totalArea += (distance * distance) * osg::PI;
    }
    return 10000000000*(std::abs(totalArea / 2.0));
    }
    return 0;

}

inline float  computeTriangleArea(float a, float b, float c)
{
    float  p = (a + b + c) / 2;
    float  s = sqrt(p * (p - a) * (p - b) * (p - c));

    return s;
}

double Polygon::CalculateAreaOfPolygon_I()
{
    double totalArea = 0.0;
    double numPoints = getSize();
    qDebug()<<numPoints;
    if (getSize()>3){
    for (unsigned int i = 1; i < numPoints-1; i++)
    {
        osg::Vec3  pos1 = mPolygonGeom->at(0);
        osg::Vec3  pos2 = mPolygonGeom->at(i);
        osg::Vec3  pos3 = mPolygonGeom->at(i+1);

        float  length1 = (pos1 - pos2).length();
        float  length2 = (pos3 - pos2).length();
        float  length3 = (pos1 - pos3).length();

        totalArea += computeTriangleArea(length1, length2, length3);
    }
    return totalArea;

}
    return 0;
}

bool Polygon::showArea() const
{
    return mShowArea;
}

void Polygon::setShowArea(bool newShowArea)
{
    if (mShowArea == newShowArea)
    return;

//    if(newShowArea)
//    mCount++;
//    else if(mCount > 0)
//    mCount--;

    mShowArea = newShowArea;

//    if (mCount > 0){
//    mLabelGroup->setNodeMask(true);
//    for(auto& data:mVecLabelData)
//    {
//        if(data.qImage)
//            delete data.qImage;
//        QImage* qImage = createOrUpdateLabelImg(data.image, data.area, data.volume);
//        data.qImage = qImage;
//        data.placeNode->setIconImage(data.image);
//        data.placeNode->setStyle(data.placeNode->getStyle());
//    }
//    }
//    else
    //mLabelGroup->setNodeMask(false);

}

bool Polygon::showVolume() const
{
    return mShowVolume;
}

void Polygon::setShowVolume(bool newShowVolume)
{
    mShowVolume = newShowVolume;
}

void Polygon::createOrUpdateLabelImg(osg::ref_ptr<osg::Image> &image, double area)
{
//    int imageHeight = 23;
//    if (mCount > 0)
//    imageHeight = mCount*23;
//    else
//    return nullptr;
    int areaPos = 5;
    int volumePos = 5;

    QImage* lblImage = new QImage(
        LABEL_IMAGE_WIDTH,
        LABEL_IMAGE_HEIGHT,
        QImage::Format_RGBA8888);
    if(!image->valid())
    image = new osg::Image;
    {

    lblImage->fill(QColor(Qt::transparent));
    QPainter painter(lblImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));
    static const QFont textFont("SourceSansPro", 10, QFont::Normal);
    static const QPen  textPen(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(
        lblImage->rect(),
        8,8);

    painter.setPen(textPen);
    painter.setFont(textFont);


        volumePos += 22;
        if (area >= 1000000){
            area/=1000000;
            QString str = QObject::tr("a: %1 km2").arg(area,0,'f',2);
            painter.drawText(8, areaPos, LABEL_IMAGE_WIDTH, 20,
                             Qt::AlignLeft|Qt::AlignVCenter,
                             str);
        }
        else{
            QString str = QObject::tr("a: %1 m2").arg(area,0,'f',2);
            painter.drawText(8, areaPos, LABEL_IMAGE_WIDTH, 20,
                             Qt::AlignLeft|Qt::AlignVCenter,
                             str);
        }

    }
    *lblImage = lblImage->mirrored(false, true);
    image->setImage(LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
                    1,
                    GL_RGBA,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    lblImage->bits(),
                    osg::Image::AllocationMode::NO_DELETE);

}

void Polygon::setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp)
{
    auto style = this->getStyle();
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping()= clamp;
    this->setStyle(style);
}

void Polygon::addPoints(osgEarth::GeoPoint point)
{


//    mPoints.push_back(point);


//    double x_sum = 0.0;
//    double y_sum = 0.0;
//    for (auto point : mPoints) {
//    x_sum += point.x();
//    y_sum += point.y();
//    }

//    mCenter.x() = x_sum / mPoints.size();
//    mCenter.y() = y_sum / mPoints.size();

    auto fea = this->getFeature();
    mPolygonGeom->push_back(point.vec3d());
    dirty();
    fea->setGeometry(mPolygonGeom);
    if(mShowArea){
    auto area = CalculateAreaOfPolygon();
    osg::ref_ptr<osg::Image> image = new osg::Image;
    createOrUpdateLabelImg(image, area);
    mPlaceNode = new osgEarth::Annotation::PlaceNode();
//    LabelData data;
//    data.qImage = qImage;
//    data.image = image;
//    data.area = area;
//    data.volume = 0;
//    data.placeNode = placeNode;
//    mVecLabelData.push_back(data);
    if(mPolygonGeom->size() >2){
        osgEarth::GeoPoint midPoint(mMapItem->getMapSRS(),
                                (mPolygonGeom->at(mPolygonGeom->size() - 2)
                                 + mPolygonGeom->at(mPolygonGeom->size() -1 )) / 2);
        mPlaceNode->setPosition(midPoint);
    }

    mPlaceNode->setIconImage(image);
    mPlaceNode->setStyle(mPlaceNode->getStyle());
    mPlaceNode->setNodeMask(true);

    addChild(mPlaceNode);
    }
    //mLabelGroup->addChild(placeNode);
    //addChild(mLabelGroup);
}
void Polygon::clearPoints()
{
    mPolygonGeom->clear();
//    for(auto labelData: mVecLabelData){
//    if(labelData.qImage)
//        delete labelData.qImage;
//    if(mPlaceNode){
//        removeChild(mPlaceNode);
//    }
//    }
//    mVecLabelData.clear();
    dirty();
    //mLabelGroup->removeChildren(0,mLabelGroup->getNumChildren());



//    dirty();
}

void Polygon::removePoint()
{
    mPolygonGeom->pop_back();


//    auto qImage = mVecLabelData[getSize()-1].qImage;
//    if(qImage)
//        delete  qImage;
//    mLabelGroup->removeChild(getSize()-1);


//    mVecLabelData.pop_back();
    dirty();
    //mLabelGroup->removeChild(getSize()-1);

//    addChild(mLabelGroup);
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
