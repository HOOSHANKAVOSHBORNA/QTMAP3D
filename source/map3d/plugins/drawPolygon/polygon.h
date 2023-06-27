#ifndef POLYGON_H
#define POLYGON_H

#include <osgEarthAnnotation/FeatureNode>
#include "mapItem.h"
#include <osgEarthAnnotation/PlaceNode>

class Polygon : public osgEarth::Annotation::FeatureNode
{
public:
    Polygon(MapItem *mapItem);
    ~Polygon();
    double getSize();
    void setLineColor(osgEarth::Color color);
    osgEarth::Color getLineColor();
    void setFillColor(osgEarth::Color color);
    osgEarth::Color getFillColor();
    void setLineWidth(float width);
    float getLineWidth();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    void addPoints(osgEarth::GeoPoint point);
    void clearPoints();
    void removePoint();
    void setHeight(float height);
    float getHeight();
    double CalculateAreaOfPolygon();
    double CalculateAreaOfPolygon_I();

    bool showArea() const;
    void setShowArea(bool newShowArea);

    bool showVolume() const;
    void setShowVolume(bool newShowVolume);

private:
    MapItem* mMapItem{nullptr};
    osg::ref_ptr<osgEarth::Features::Geometry> mPolygonGeom;

private:
    void createOrUpdateLabelImg(osg::ref_ptr<osg::Image> &image, double area);
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mPlaceNode;

//    struct LabelData {
//        QImage *qImage{nullptr};
//        osg::ref_ptr<osg::Image> image;
//        double area;
//        double volume;
//        osg::ref_ptr<osgEarth::Annotation::PlaceNode> placeNode;
//    };
    //osg::ref_ptr<osg::Group> mLabelGroup;
    //std::vector<LabelData> mVecLabelData;
    bool mShowArea{false};
    bool mShowVolume{false};
    static constexpr int LABEL_IMAGE_WIDTH = 125;
    static constexpr int LABEL_IMAGE_HEIGHT = 30;
    int mCount{0};
    QList<osgEarth::GeoPoint> mPoints;
    osgEarth::GeoPoint mCenter;
};



#endif // POLYGON_H
