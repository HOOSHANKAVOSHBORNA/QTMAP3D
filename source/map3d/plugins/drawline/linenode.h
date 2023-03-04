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
    osgEarth::Symbology::Geometry* mLineGeometry;
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


protected:
    osg::ref_ptr<osg::Group> mLableGroup;
private:
    MapController* mMapController{nullptr};

    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth{5};
    float mPointWidth{5};
    float mHeight;
    bool mPointVisible{true};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};

    //Lenght part
    osg::Image *updateLenghtLable(double lenght);
    osgEarth::Symbology::Style labelStyle;
    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 100;
    static constexpr int LABEL_IMAGE_HEIGHT = 20;
};

#endif // LINENODE_H
