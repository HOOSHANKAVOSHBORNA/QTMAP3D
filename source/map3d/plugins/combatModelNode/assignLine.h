#ifndef ASSIGNLINE_H
#define ASSIGNLINE_H

#include <osgEarth/LineDrawable>
#include "annotatedNode.h"
#include "simpleModelNode.h"

class AssignLineCallback : public osg::NodeCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

class AssignLine : public AnnotatedNode
{
public:
    AssignLine(MapItem *mapItem, SimpleModelNode *attacker, SimpleModelNode *target);
    void update();
private:
    osg::ref_ptr<SimpleModelNode> mAttacker;
    osg::ref_ptr<SimpleModelNode> mTarget;
};

#endif // ASSIGNLINE_H
