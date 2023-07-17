#include "utility.h"

Utility::Utility()
{

}

QColor Utility::osgEarthColorToQColor(const osgEarth::Color &color)
{
    return QColor(color.r()*255, color.g()*255, color.b()*255, color.a()*255);
}

osgEarth::Color Utility::qColor2osgEarthColor(const QColor &color){
    return osgEarth::Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}
