//#ifndef POLYGON_H
//#define POLYGON_H

//#include <osgEarthAnnotation/FeatureNode>
//#include "mapItem.h"
//#include "serviceManager.h"
//#include <osgEarthAnnotation/PlaceNode>

//class Polygon : public osgEarth::Annotation::FeatureNode
//{
//public:
//    Polygon(MapItem *mapItem);
//    ~Polygon();
//    void create(std::vector<osg::Vec3d> *points);
//    void addPoint(osgEarth::GeoPoint point);
//    void clearPoints();
//    void removePoint();
//    double getSize();

//    void setFillColor(osgEarth::Color color);
//    osgEarth::Color getFillColor() const;

//    void setStrokeColor(osgEarth::Color color);
//    osgEarth::Color getStrokeColor() const;
//    void setStrokeWidth(float width);
//    float getStrokeWidth() const;

//    void setHeight(float height);
//    float getHeight() const;

//    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp() const;
//    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);

//    bool getShowArea() const;
//    void setShowArea(bool newShowArea);
//    bool getShowVolume() const;
//    void setShowVolume(bool newShowVolume);

//    PolygonData *polygonData() const;
//    void setPolygonData(PolygonData *newPolygonData);

//private:
//    double CalculateAreaOfPolygon();
//    double CalculateAreaOfPolygon_I();
//    void createOrUpdateLabelImg(osg::ref_ptr<osg::Image> &image, double area);

//private:
//    MapItem* mMapItem{nullptr};
//    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mPlaceNode;

//    osgEarth::Color mFillColor{osgEarth::Color::White};
//    osgEarth::Color mStrokeColor{osgEarth::Color::White};
//    float mStrokeWidth{5};
//    float mHeight{0};
//    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
//    bool mShowArea{false};
//    bool mShowVolume{false};

//    static constexpr int LABEL_IMAGE_WIDTH = 125;
//    static constexpr int LABEL_IMAGE_HEIGHT = 30;
//    int mCount{0};
//    QList<osgEarth::GeoPoint> mPoints;
//    osgEarth::GeoPoint mCenter;
//    PolygonData *mPolygonData{nullptr};
//};



//#endif // POLYGON_H
