#ifndef SPHERENODEEDITOR_H
#define SPHERENODEEDITOR_H

#include "spherenode.h"

#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/Draggers>

class SphereNodeEditor: public osgEarth::Annotation::GeoPositionNodeEditor
{
public:
    SphereNodeEditor(SphereNode* sphereNode);
    void computeBearing();
    virtual void updateDraggers();

    osgEarth::Annotation::Dragger* mRadiusDragger;
    SphereNode* mSphereNode{nullptr};
    double mBearing;
};

#endif // SPHERENODEEDITOR_H
