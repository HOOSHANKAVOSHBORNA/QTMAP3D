
#include <iostream>
#include "test1plugin.h"
#include "mapcontroller.h"

Test1Plugin::Test1Plugin()
{
    Q_INIT_RESOURCE(test1plugin);
}

bool Test1Plugin::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc)
{
    pDesc->pluginHasSideItem = true;
    pDesc->sideItemMenuBarTitle = "Layers";
    pDesc->sideItemMenuBarIconUrl = "qrc:///test1plugin/resources/Layers.png";
    pDesc->sideItemUrl = "qrc:///test1plugin/Layers.qml";

    pDesc->toolboxItemsList.push_back(new ToolboxItemDesc{"One", "Two", "", true, false, ""});

    return true;
}

void Test1Plugin::onSideItemCreated(int index, QObject *pSideItem)
{

}

void Test1Plugin::onToolboxItemClicked(const QString& name, const QString& category)
{
    auto mBackVisibilityNode = makeBackground(mMapController, 20000.0f);
    osgEarth::GeoPoint  geoPos(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.461);
    mBackVisibilityNode->setPosition(geoPos);
    mMapController->addNode(mBackVisibilityNode);

    //Set view point------------------------------------------------------------------
    mMapController->goToPosition(geoPos.x(), geoPos.y(), 100000);
}

bool Test1Plugin::initialize3D(MapController *pMapController)
{
    mMapController = pMapController;

    return true;
}

osgEarth::Annotation::ModelNode *Test1Plugin::makeBackground(MapController *pMapController, float radius)
{
    osg::Vec3 eye(0.3f,0.2f,1.0f);
    osg::ref_ptr<osg::Sphere>         pSphereShape   = new osg::Sphere(eye, radius);
    osg::ref_ptr<osg::ShapeDrawable>  pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());

    pShapeDrawable->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.3f));

    osg::ref_ptr<osg::Geode>  geode = new osg::Geode();
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);

    geode->addDrawable(pShapeDrawable.get());



//    osg::ref_ptr<osg::Node> mDrownode = makeShape(osg::Vec3(position.x(),position.y(),position.z()),20000.0f);
    osgEarth::Symbology::Style  style;

    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(geode);

    osg::ref_ptr<osgEarth::Annotation::ModelNode>  model;
    model = new osgEarth::Annotation::ModelNode(pMapController->getMapNode(), style);

    return model.release();
}
