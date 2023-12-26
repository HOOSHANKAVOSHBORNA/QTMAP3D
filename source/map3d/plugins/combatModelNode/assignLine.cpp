#include "assignLine.h"

void AssignLineCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    AssignLine *assignLine = static_cast<AssignLine*>(node);
    assignLine->update();
    traverse(node, nv);
}

AssignLine::AssignLine(MapItem *mapItem, SimpleModelNode* attacker, SimpleModelNode *target)
    : LineNode(mapItem),
    mAttacker(attacker),
    mTarget(target)
{
    setWidth(5);
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
    create(&points);
//    setTessellation(100);
//    setPointVisible(true);
//    setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping::CLAMP_TO_TERRAIN);
//    clear();
//    addPoint(mAttacker->getPosition());
//    addPoint(mTarget->getPosition());
}

//void AssignLine::setState(AssignState state)
//{
//    switch (state) {
//    case PREASSIGN:
//        mLine->setMode(GL_LINES);
//        mLine->setColor(osg::Vec4(0.00392156862745098,0.6823529411764706,0.8392156862745098,1));
//        mLine->setLineWidth(8);
//        break;
//    case ASSIGNED:
//        mLine->setMode(GL_LINE_STRIP);
//        mLine->setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
//        mLine->setLineWidth(8);
//        break;
//    case SEARCH:
//        mLine->setMode(GL_LINE_STRIP);
//        mLine->setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
//        mLine->setLineWidth(8);
//        break;
//    case LOCK:
//        mLine->setMode(GL_LINE_SMOOTH);
//        mLine->setColor(osg::Vec4(0.4196078431372549,0,0,1));
//        mLine->setLineWidth(8);
//        break;
//    case FIRE:
//        mLine->setMode(GL_LINE_STRIP);
//        mLine->setColor(osg::Vec4(1,0,0,1));
//        mLine->setLineWidth(8);
//        break;
//    case BUSY:
//        mLine->setMode(GL_POINTS);
//        mLine->setColor(osg::Vec4(0.5882352941176471,0.5882352941176471,0.5882352941176471,1));
//        mLine->setLineWidth(8);
//        break;
//    case SUCCEED:
//        mLine->setMode(GL_LINES);
//        mLine->setColor(osg::Vec4(0,1,0,1));
//        mLine->setLineWidth(8);
//        break;
//    case FAILED:
//        mLine->setMode(GL_LINES);
//        mLine->setColor(osg::Vec4(0.9215686274509803,0.4235294117647059,0.4235294117647059,1));
//        mLine->setLineWidth(8);
//        break;
//    default:
//        break;
//    }
//}


