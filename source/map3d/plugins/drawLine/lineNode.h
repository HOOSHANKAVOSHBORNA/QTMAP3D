#ifndef LINENODE_H
#define LINENODE_H

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

private:

    QImage *createOrUpdateLabelImg(osg::ref_ptr<osg::Image> &image, double lenght, double bearing, double slope);

    struct LabelData {
        QImage *qImage{nullptr};
        osg::ref_ptr<osg::Image> image;
        double lenght;
        double bearing;
        double slope;
        osg::ref_ptr<osgEarth::Annotation::PlaceNode> placeNode;
        //		LabelData() = default;
        //		LabelData(const LabelData& source){
        //			qImage = source.qImage;
        //			image = source.image;
        //			lenght = source.lenght;
        //			bearing = source.bearing;
        //			placeNode = source.placeNode;
        //		}
        //		LabelData(LabelData&& source){
        //			qImage = source.qImage;
        //			image = source.image;
        //			lenght = source.lenght;
        //			bearing = source.bearing;
        //			placeNode = source.placeNode;

        //			source.qImage = nullptr;
        //		}
        //		LabelData& operator=(const LabelData& source){
        //			LabelData* data = new LabelData ();
        //			data->qImage = source.qImage;
        //			data->image = source.image;
        //			data->lenght = source.lenght;
        //			data->bearing = source.bearing;
        //			data->placeNode = source.placeNode;
        //			return *data;
        //		}
        //		~LabelData(){
        //			if(qImage){
        //				delete qImage;
        //				qImage = nullptr;
        //			}
        //		}

    };

private:
    MapItem* mMapItem{nullptr};
    osgEarth::Symbology::Geometry* mLineGeometry{nullptr};
    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth{5};
    float mPointWidth{5};
    float mHeight{0};
    bool mShowBearing{false};
    bool mShowSlope{false};
    bool mShowDistance{false};

    bool mPointVisible{false};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};
    bool mSmooth{true};
    std::vector<LabelData> mVecLabelData;
    //Lenght part
    osg::ref_ptr<osg::Group> mLabelGroup;
    static constexpr int LABEL_IMAGE_WIDTH = 100;
    static constexpr int LABEL_IMAGE_HEIGHT = 27;
    int mCount{0};

};

#endif // LINENODE_H
