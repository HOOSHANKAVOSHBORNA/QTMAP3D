#ifndef LINENODE_H
#define LINENODE_H

#include "statusNode.h"
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapItem.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>
#include <QPainter>
#include <osgEarthAnnotation/PlaceNode>
#include <QImage>

class LineNode : public osgEarth::Annotation::FeatureNode
{
public:
    LineNode(MapItem *mapItem);
    ~LineNode();
    void create(std::vector<osg::Vec3d> *points);
    void addPoint(osgEarth::GeoPoint point);
    osgEarth::GeoPoint getPoint(int index);
    void removePoint();
    void removeFirstPoint();
    void clear();
    int getSize();
    osgEarth::Color getFillColor() const;
    void setFillColor(const osgEarth::Color &color);

    float getWidth() const;
    void setWidth(float width);

    float getHeight() const;
    void setHeight(float height);

    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp);

    unsigned getTessellation() const;
    void setTessellation(const unsigned &tessellation);

    //----------------------------------------------------------
    bool getPointVisible() const;
    void setPointVisible(bool value);

    bool getShowBearing() const;
    void setShowBearing(const bool &bearing);

    void setShowDistance(const bool &show);
    bool getShowDistance() const;

    bool getSmooth() const;
    void setSmooth(bool Smooth);

    osgEarth::Color getPointColor() const;
    void setPointColor(const osgEarth::Color &pointColor);

    float getPointWidth() const;
    void setPointWidth(float pointWidth);

    bool getShowSlope() const;
    void setShowSlope(bool showSlope);

    bool getShowAltitude() const;
    void setShowAltitude(bool showAltitude);

private:
    struct LabelData {
//        QImage *qImage{nullptr};
//        osg::ref_ptr<osg::Image> image;
        double distance;
        double altitude;
        double bearing;
        double slope;
        osg::ref_ptr<StatusNode> statusNode;
    };

    void updateStatusNode(LabelData &lableData);

private:
    MapItem* mMapItem{nullptr};
//    osgEarth::Symbology::Geometry* mLineGeometry{nullptr};
    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth{5};
    float mPointWidth{5};
    float mHeight{0};
    bool mShowBearing{false};
    bool mShowSlope{false};
    bool mShowDistance{false};
    bool mShowAltitude{false};

    bool mPointVisible{false};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};
    bool mSmooth{true};
    std::vector<LabelData> mVecLabelData;
    //Lenght part
    osg::ref_ptr<osg::Group> mLabelGroup;
//    static constexpr int LABEL_IMAGE_WIDTH = 100;
//    static constexpr int LABEL_IMAGE_HEIGHT = 27;
    int mCount{0};

};

#endif // LINENODE_H
