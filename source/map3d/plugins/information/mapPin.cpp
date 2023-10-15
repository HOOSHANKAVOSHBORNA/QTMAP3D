#include "mapPin.h"
#include <osg/Material>
#include <osg/Depth>
#include <osgEarthAnnotation/AnnotationUtils>


MapPin::MapPin(MapItem* map , osg::Vec4f color)
    : osgEarth::Annotation::ModelNode(map->getMapNode(), MapPin::getDefaultStyle()),
    mMapItem(map)
{
    connect(map, &MapItem::modeChanged, this, &MapPin::onModeChanged);
    is3D = map->getMode();
//    mScaler = new ModelAutoScaler(15, 2, 500);
    //--3D node----------------------------------------------------------
    m3DPin = osgDB::readRefNodeFile("../data/models/Qarch1.osgb");
    //--3D model Color --------------------------------------------------
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, color);
    m3DPin->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //--status-----------------------------------------------------------
    mStatus = new StatusNode(map);
    NodeFieldData lat;
    lat.name = "Latitude";
    lat.value = getPosition().x();
    mDataList.push_back(lat);
    NodeFieldData lon;
    lon.name = "Longtitude";
    lon.value = getPosition().y();
    mDataList.push_back(lon);
    mTitle = "Map Pin";
//    mStatus->setData(mTitle,mDataList);
    //--root ------------------------------------------------------------
    mSwitchNode = new osg::Switch;
    //--2D node---------------------------------------------------------
    m2DPin = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/location/mappin.png");
    for(unsigned int i = 0; i < 64; i++) {
        for(unsigned int j = 0; j < 64; j++)
        {
            const osg::Vec4 pixColore = image->getColor(i, j);
            if(pixColore.a() > 0)
                image->setColor(color, i, j);
            else
                image->setColor(osg::Vec4(0,0,0,0), i, j);
        }
    }
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(image, osg::Vec2s(0,0), 0, 0, 0.2);
    m2DPin->setStateSet(geodeStateSet);
    m2DPin->addDrawable(imgGeom);
    //--setting 3D 2D view-----------------------------------------------
    mSwitchNode->addChild(mStatus);
    if(is3D){
        mSwitchNode->addChild(m3DPin, true);
        mSwitchNode->addChild(m2DPin, false);
    }
    else{
        mSwitchNode->addChild(m3DPin, false);
        mSwitchNode->addChild(m2DPin, true);
    }
    //--add to model node and AutoScale-----------------------------------------------
    getGeoTransform()->addChild(mSwitchNode);
    setCullingActive(false);
//    addCullCallback(mScaler);
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mSwitchNode);
    setStyle(rootStyle);
}


osgEarth::Symbology::Style MapPin::getDefaultStyle()
{
    static osgEarth::Symbology::Style _style;
    static bool bFirst = true;
    if (bFirst) {
        static osg::Node *node = new osg::Node;
        _style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
        bFirst = false;
    }
    return _style;
}



void MapPin::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
    }
}

