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

    osgEarth::Color getPointColor() const;
    void setPointColor(const osgEarth::Color &pointColor);

    float getPointWidth() const;
    void setPointWidth(float pointWidth);

    bool getSmooth() const;
    void setSmooth(bool Smooth);

private:
    osg::Image *updateLenghtLabel(double lenght);
private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLineGeometry{nullptr};
    osgEarth::Color mColor{osgEarth::Color::Green};
    osgEarth::Color mPointColor{osgEarth::Color::Blue};
    float mWidth{5};
    float mPointWidth{5};
    float mHeight{0};
    bool mPointVisible{false};
    osgEarth::Symbology::AltitudeSymbol::Clamping mClamp;
    unsigned mTessellation{1};
    bool mSmooth{true};

    //Lenght part
    osg::ref_ptr<osg::Group> mLabelGroup;
    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 100;
    static constexpr int LABEL_IMAGE_HEIGHT = 20;
    bool mIsHeight{false};

};

#endif // LINENODE_H
