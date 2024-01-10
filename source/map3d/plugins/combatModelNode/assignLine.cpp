#include "assignLine.h"

void AssignLineCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    AssignLine *assignLine = static_cast<AssignLine*>(node);
    assignLine->update();
    traverse(node, nv);
}

AssignLine::AssignLine(MapItem *mapItem, SimpleModelNode* attacker, SimpleModelNode *target)
    : AnnotatedNode(mapItem, AnnotatedNode::GeneralType::POLYLINETYPE),
    mAttacker(attacker),
    mTarget(target)
{
    setWidth(3);
    setPointVisible(true);
    setPointWidth(20);
    update();
    addCullCallback(new AssignLineCallback);
}

void AssignLine::update()
{
    std::vector<osg::Vec3d> points;
    auto attackPos = mAttacker->getPosition().vec3d();
    attackPos.z() += mAttacker->getBound().radius()/2;
    auto targetPos = mTarget->getPosition().vec3d();
    targetPos.z() += mAttacker->getBound().radius()/2;

    points.push_back(attackPos);
    points.push_back(targetPos);
    create(points);
}
