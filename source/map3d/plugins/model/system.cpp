#include "system.h"
#include "truck.h"

#include <osgEarthAnnotation/AnnotationUtils>

#include <osg/Material>

System::System(osgEarth::MapNode *mapNode, QObject *parent)
    :BaseModel(mapNode, parent)
{
    //create switch node for root--------------------------------------------------------------------
    mRoot = new osg::Switch;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRoot);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
    //--create icon Nodes---------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/system/system_red.png");
    redIcon->scaleImage(32, 32, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    redGeode->addDrawable(redImageDrawable);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/system/system_yell.png");
    yellowIcon->scaleImage(32, 32, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);
    //truck model--------------------------------------------------------------------------
    osg::ref_ptr<Truck> truck = new Truck(getMapNode());
    //--add nods--------------------------------------------------------------------------------
    mRoot->addChild(truck, false);
    mRoot->addChild(redGeode,true);
    mRoot->addChild(yellowGeode, false);
    //----------------------------------------------------------------------------------------
    mCameraRangeChangeable = true;
}

