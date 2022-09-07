#include "objecttracker.h"
#include "map3dwidget.h"

#include <QAction>
#include <QToolBar>
#include <QDebug>

#include <osgDB/ReadFile>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthAnnotation/ModelNode>
#include <osg/PositionAttitudeTransform>
#include <osgEarth/GeoData>

ObjectTracker::ObjectTracker(QWidget *parent)
    : PluginInterface(parent)
{
}

void ObjectTracker::setUpUI()
{
//    const QIcon readIcon (":/res/read.png");
    auto trackAction = new QAction("Traking");
    trackAction->setToolTip("Tracking a motion object.");
    mToolBar->addAction(trackAction);
    QObject::connect(trackAction, &QAction::triggered, this, &ObjectTracker::trackObject);
}

void ObjectTracker::trackObject()
{
    qDebug() << "track object";

    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../map3dlib/data/models/tree.osgb");

    if (!node)
    {
        return;
    }
    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);

    osg::ref_ptr<osgEarth::Annotation::ModelNode>  model;
    model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);

    //auto srs = mMap3dWidget->getMapNode()->getMap()->getWorldSRS();
    //osgEarth::GeoPoint pos(srs,52.859, 35.241);
    osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.241, 800);

    model->setPosition(point);
    model->setScale(osg::Vec3(10,10,10));

    mMap3dWidget->getMapNode()->addChild(model);

    osgEarth::Viewpoint vp;
    vp.focalPoint()= point;
    vp.range()=500;
    vp.heading()->set(50, osgEarth::Units::DEGREES);
    //vp.pitch()->set(-25, osgEarth::Units::DEGREES);
    mMap3dWidget->setViewpoint(vp, 10);
}
