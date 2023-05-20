#ifndef LINENODE_H
#define LINENODE_H

#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>
#include <QPainter>
#include <osgEarthAnnotation/PlaceNode>
#include <QImage>

class LineNode : public osgEarth::Annotation::FeatureNode
{
public:
    LineNode(MapController *mapController);
    void addPoint(osgEarth::GeoPoint point);
    void removePoint();
    void removeFirstPoint();
    void clear();
    int getSize();
    osgEarth::Color getColor() const;
    void setColor(const osgEarth::Color &color);

    float getWidth() const;
    void setWidth(float width);

    float getHeight() const;
    void setHeight(float height);

    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp);

    unsigned getTessellation() const;
    void setTessellation(const unsigned &tessellation);

    bool getIsHeight() const;
    void setIsHeight(bool value);
    //----------------------------------------------------------
    bool getPointVisible() const;
    void setPointVisible(bool value);

    bool getShowBearing() const;
    void setShowBearing(const bool &bearing);

    void setShowLenght(const bool &show);
    bool getShowLenght() const;

    osgEarth::Color getPointColor() const;
    void setPointColor(const osgEarth::Color &pointColor);

    float getPointWidth() const;
    void setPointWidth(float pointWidth);

    bool getSmooth() const;
    void setSmooth(bool Smooth);

    bool getShowSlope() const;
    void setShowSlope(bool showSlope);

private:
    void createOrUpdateLabelImg(QImage** qImage, osg::ref_ptr<osg::Image> &img,
                                double lenght, double bearing, double slope);
    struct LabelData {
        QImage** qImage;
        osg::ref_ptr<osg::Image> img;
        double lenght;
        double bearing;
        double slope;
        osg::ref_ptr<osgEarth::Annotation::PlaceNode> placeNode;

    };

private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLineGeometry{nullptr};
    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth{5};
    float mPointWidth{5};
    float mHeight{0};
    bool mShowBearing{false};
    bool mShowLenght{false};
    bool mShowSlope{true};
    bool mPointVisible{false};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};
    bool mSmooth{true};
    std::vector<LabelData> mVecLabelData;

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mPlaceNode;

    //Lenght part
    osg::ref_ptr<osg::Group> mLabelGroup;
    static constexpr int LABEL_IMAGE_WIDTH = 100;
    static constexpr int LABEL_IMAGE_HEIGHT = 30;
    int mHeightLbl;
    bool mIsHeight{false};
    int mBeaPos;
    int mSlopePos;
    int mCount{0};
};

#endif // LINENODE_H
