#ifndef UTILITY_H
#define UTILITY_H

#include <QColor>
#include <osgEarth/Color>


class Utility
{
public:
    Utility();
    static QColor osgEarthColorToQColor(const osgEarth::Color &color);
    static osgEarth::Color qColor2osgEarthColor(const QColor &color);
};

#endif // UTILITY_H
