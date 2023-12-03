#include "assignLine.h"
#include "circle.h"

AssignLine::AssignLine(MapItem *map):
    LineNode(map)
{
    mMapItem = map;

    auto style = getStyle();
    // style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = mColor;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->lineCap() = osgEarth::Symbology::Stroke::LINECAP_ROUND;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 20;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 50;
    // style.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = osgEarth::Color(0.5,0.3,0.4,1.0);
    // style.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = 10;
    // style.getOrCreate<osgEarth::Symbology::PointSymbol>()
    // osg::ref_ptr<Circle> mCircleHighlightNode = new Circle();
    // mCircleHighlightNode->setFillColor(osg::Vec4f(0,0.0,0.0,0));
    // mCircleHighlightNode->setStrokeColor(osg::Vec4f(1,0.20784313725490197,0.0,0.8));
    // mCircleHighlightNode->setStrokeWidth(2);
    // mCircleHighlightNode->setRadius(2000);
    // mCircleHighlightNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.5));
    // style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mCircleHighlightNode);
    setStyle(style);
}


osg::Image* AssignLine::createSpotLight( const osg::Vec4& centerColor,unsigned int size, float power )
{
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage( size, size, 1, GL_RGBA,GL_UNSIGNED_BYTE );
    float mid = (float(size)-1) * 0.5f;
    float div = 2.0f / float(size);
    for( unsigned int r=0; r<size; ++r )
    {
        unsigned char* ptr = image->data(0, r);
        for( unsigned int c=0; c<size; ++c )
        {
            float dx = (float(c) - mid)*div;
            float dy = (float(r) - mid)*div;
            float r = powf(1.0f - sqrtf(dx*dx+dy*dy), power);
            if ( r<0.0f ) r = 0.0f;
            osg::Vec4 color = centerColor*r + centerColor*(1.0f - r);
            *ptr++ = (unsigned char)((color[0]) * 255.0f);
            *ptr++ = (unsigned char)((color[1]) * 255.0f);
            *ptr++ = (unsigned char)((color[2]) * 255.0f);
            *ptr++ = (unsigned char)((color[3]) * 255.0f);
        }
    }
    return image.release();
}
