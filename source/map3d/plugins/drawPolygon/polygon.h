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
    void addPoint(osgEarth::GeoPoint point);
    void clearPoints();
    void removePoint();
    double getSize()const;

    void setFillColor(osgEarth::Color color);
    osgEarth::Color getFillColor() const;

    void setStrokeColor(osgEarth::Color color);
    osgEarth::Color getStrokeColor() const;
    void setStrokeWidth(float width);
    float getStrokeWidth() const;

    void setHeight(float height);
    float getHeight() const;

    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp() const;
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);

    bool getShowArea() const;
    void setShowArea(bool newShowArea);
    bool getShowVolume() const;
    void setShowVolume(bool newShowVolume);

private:
    double CalculateAreaOfPolygon();
    double CalculateAreaOfPolygon_I();
    void createOrUpdateLabelImg(osg::ref_ptr<osg::Image> &image, double area);

private:
    MapItem* mMapItem{nullptr};
    osg::ref_ptr<osgEarth::Features::Geometry> mPolygonGeom;
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
    osgEarth::Color mFillColor{osgEarth::Color::Green};
    osgEarth::Color mStrokeColor{osgEarth::Color::Green};
    float mStrokeWidth{5};
    float mHeight{0};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    bool mShowArea{false};
    bool mShowVolume{false};

    static constexpr int LABEL_IMAGE_WIDTH = 125;
    static constexpr int LABEL_IMAGE_HEIGHT = 30;
    int mCount{0};
    QList<osgEarth::GeoPoint> mPoints;
    osgEarth::GeoPoint mCenter;
};



#endif // POLYGON_H
