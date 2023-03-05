#ifndef LINENODE_H
#define LINENODE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>
#include <QPainter>
#include <osgEarthAnnotation/PlaceNode>

class LineNode : public osgEarth::Annotation::FeatureNode
{
public:
    LineNode(MapController *mapController);
    void addPoint(osgEarth::GeoPoint point);
    void removePoint();
    void removeFirstPoint();
    void clear();
    int getSize();
    void showLenght(bool show);

    osgEarth::Color getColor() const;
    void setColor(const osgEarth::Color &color);

    osgEarth::Color getPointColor() const;
    void setPointColor(const osgEarth::Color &pointColor);

    float getWidth() const;
    void setWidth(float width);

    float getHeight() const;
    void setHeight(float height);

    bool getPointVisible() const;
    void setPointVisible(bool value);
    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp() const;
    void setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &clamp);

    unsigned getTessellation() const;
    void setTessellation(const unsigned &tessellation);

    float getPointWidth() const;
    void setPointWidth(float pointWidth);

    bool getSmooth() const;
    void setSmooth(bool Smooth);
private:
    osg::Image *updateLenghtLable(double lenght);
private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLineGeometry;
    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth;
    float mPointWidth;
    float mHeight;
    bool mPointVisible;
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};
    bool mSmooth;

    //Lenght part
    osg::ref_ptr<osg::Group> mLableGroup;
    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 100;
    static constexpr int LABEL_IMAGE_HEIGHT = 20;
};

#endif // LINENODE_H
