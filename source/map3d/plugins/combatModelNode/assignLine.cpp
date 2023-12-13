#include "assignLine.h"


AssignLine::AssignLine(osgEarth::GeoPoint start, osgEarth::GeoPoint end, AssignState state)
{
    mLine = new osgEarth::LineDrawable();
    setLinePosition(start,end);
    setState(state);
    addChild(mLine);
}

void AssignLine::setState(AssignState state)
{
    switch (state) {
    case PREASSIGN:
        mLine->setMode(GL_POINTS);
        mLine->setColor(osg::Vec4(0.00392156862745098,0.6823529411764706,0.8392156862745098,1));
        mLine->setLineWidth(8);
        break;
    case ASSIGNED:
        mLine->setMode(GL_LINE_STRIP);
        mLine->setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
        mLine->setLineWidth(8);
        break;
    case SEARCH:
        mLine->setMode(GL_LINE_STRIP);
        mLine->setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
        mLine->setLineWidth(8);
        break;
    case LOCK:
        mLine->setMode(GL_LINE_SMOOTH);
        mLine->setColor(osg::Vec4(0.4196078431372549,0,0,1));
        mLine->setLineWidth(8);
        break;
    case FIRE:
        mLine->setMode(GL_LINE_STRIP);
        mLine->setColor(osg::Vec4(1,0,0,1));
        mLine->setLineWidth(8);
        break;
    case BUSY:
        mLine->setMode(GL_POINTS);
        mLine->setColor(osg::Vec4(0.5882352941176471,0.5882352941176471,0.5882352941176471,1));
        mLine->setLineWidth(8);
        break;
    case SUCCEED:
        mLine->setMode(GL_LINES);
        mLine->setColor(osg::Vec4(0,1,0,1));
        mLine->setLineWidth(8);
        break;
    case FAILED:
        mLine->setMode(GL_LINES);
        mLine->setColor(osg::Vec4(0.9215686274509803,0.4235294117647059,0.4235294117647059,1));
        mLine->setLineWidth(8);
        break;
    default:
        break;
    }
}

void AssignLine::setLinePosition(osgEarth::GeoPoint start,osgEarth::GeoPoint end)
{
    osg::Vec3d startWorld;
    start.toWorld(startWorld);
    osg::Vec3d endWorld;
    end.toWorld(endWorld);
    mLine->pushVertex(startWorld);
    mLine->pushVertex(endWorld);
}
