#include "assignLine.h"


AssignLine::AssignLine(AssignState state):
    osgEarth::LineDrawable(GL_LINE)
{
    setState(state);

}

void AssignLine::setState(AssignState state)
{
    switch (state) {
    case PREASSIGN:
        setMode(GL_POINTS);
        setColor(osg::Vec4(0.00392156862745098,0.6823529411764706,0.8392156862745098,1));
        setLineWidth(8);
        break;
    case ASSIGNED:
        setMode(GL_LINE_STRIP);
        setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
        setLineWidth(8);
        break;
    case SEARCH:
        setMode(GL_LINE_STRIP);
        setColor(osg::Vec4(0.9882352941176471,0.7294117647058823,0.011764705882352941,1));
        setLineWidth(8);
        break;
    case LOCK:
        setMode(GL_LINE_SMOOTH);
        setColor(osg::Vec4(0.4196078431372549,0,0,1));
        setLineWidth(8);
        break;
    case FIRE:
        setMode(GL_LINE_STRIP);
        setColor(osg::Vec4(1,0,0,1));
        setLineWidth(8);
        break;
    case BUSY:
        setMode(GL_POINTS);
        setColor(osg::Vec4(0.5882352941176471,0.5882352941176471,0.5882352941176471,1));
        setLineWidth(8);
        break;
    case SUCCEED:
        setMode(GL_LINES);
        setColor(osg::Vec4(0,1,0,1));
        setLineWidth(8);
        break;
    case FAILED:
        setMode(GL_LINES);
        setColor(osg::Vec4(0.9215686274509803,0.4235294117647059,0.4235294117647059,1));
        setLineWidth(8);
        break;
    default:
        break;
    }
}


