#ifndef MEASUREHEIGHT_H
#define MEASUREHEIGHT_H

#include <osg/Group>
#include <osgEarth/GeoTransform>
#include <lineNode.h>
#include "mapItem.h"
#include <osgEarthAnnotation/PlaceNode>

class MeasureHeight : public osg::Group
{
public:
    MeasureHeight(MapItem *mapItem);
    void setFirstPoint(osgEarth::GeoPoint P1);
    void setSecondPoint(osgEarth::GeoPoint P2);
    void clear();
    void draw();
    bool started() const;

    osgEarth::Color getColor() const;
    void setColor(const osgEarth::Color &value);

    float getWidth() const;
    void setWidth(float width);

private:
    double height();
private:
    MapItem* mMapItem{nullptr};
    LineNode* mHLine{nullptr};
    LineNode* mVLine{nullptr};
    osgEarth::GeoPoint mFirstPoint;
    osgEarth::GeoPoint mSecondPoint;
    osgEarth::GeoPoint mJointPoint;
    bool mStarted{false};
    osgEarth::Color color{osgEarth::Color::Green};
    float mWidth;

private:
    //Lenght part
    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 90;
    static constexpr int LABEL_IMAGE_HEIGHT = 20;
private:
    void updateHeightLabel(double height);
     osg::ref_ptr<osg::Image> mImageLabel;
     osg::ref_ptr<osgEarth::Annotation::PlaceNode> mLabelNode;

};

#endif // MEASUREHEIGHT_H
