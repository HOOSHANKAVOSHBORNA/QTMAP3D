#include "visibility.h"
#include "mapItem.h"

const QString CATEGORY = "Analyze";
const QString VISIBILITY = "Visibility";


Visibility::Visibility(QObject *parent): PluginInterface(parent)
{
    Q_INIT_RESOURCE(visibility);
}

bool Visibility::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    Q_UNUSED(engine)
//    desc->pluginHasSideItem = true;
//    desc->sideItemMenuBarTitle = "Layers";
//    desc->sideItemMenuBarIconUrl = "qrc:///test1plugin/resources/Layers.png";
//    desc->sideItemUrl = "qrc:///test1plugin/Layers.qml";

    desc->toolboxItemsList.push_back(new ItemDesc{VISIBILITY, CATEGORY, "qrc:/resources/visibility.png", true, false, ""});

    return true;
}

void Visibility::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if(CATEGORY == category && name == VISIBILITY)
    {
        if(checked)
        {
            //QObject::connect(mMapItem,&MapItem::mouseEvent, this, &Visibility::onMouseEvent);
            mMapItem->addNode(mIconNode);
        }
        else
        {
            //QObject::disconnect(mMapItem,&MapItem::mouseEvent, this, &Visibility::onMouseEvent);

            mMapItem->removeNode(mBackVisibilityNode);
            mMapItem->removeNode(mVisibilityNode);

            mMapItem->removeNode(mIconNode);
        }
    }
}

bool Visibility::setup(MapItem *mapItem,
                       UIHandle *UIHandle)
{
    mMapItem = mapItem;
    mIconNode = makeIconNode();
    return true;
}

void Visibility::onMouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::LeftButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {

        mMapItem->removeNode(mBackVisibilityNode);
        mMapItem->removeNode(mVisibilityNode);

        geoPos.makeGeographic();
//        mMainWindow->webSocket()->sendMessage(QString::fromStdString(geoPos.toString()));

        mBackVisibilityNode = makeBackground(20000.0f);
        //osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.461);
        mBackVisibilityNode->setPosition(geoPos);
        mMapItem->addNode(mBackVisibilityNode);
        ///////////
        QVector<osg::Vec3d> vertices;
        vertices.push_back( osg::Vec3d(geoPos.x() - 0.089,geoPos.y() + 0.059, 0));
//        vertices.push_back( osg::Vec3d(52.86,35.55, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() + 0.001,geoPos.y() + 0.089, 0));
//        vertices.push_back( osg::Vec3d(52.96,35.52, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() + 0.101,geoPos.y() + 0.059, 0));
//        vertices.push_back( osg::Vec3d(53.01,35.47, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() + 0.151,geoPos.y() + 0.009, 0));
//        vertices.push_back( osg::Vec3d(52.92,35.37, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() + 0.061,geoPos.y() - 0.091, 0));
//        vertices.push_back( osg::Vec3d(52.80,35.34, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() - 0.059,geoPos.y() - 0.121, 0));
//        vertices.push_back( osg::Vec3d(52.76,35.40, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() - 0.099,geoPos.y() - 0.061, 0));
//        vertices.push_back( osg::Vec3d(52.74,35.49, 0) );
        vertices.push_back( osg::Vec3d(geoPos.x() - 0.119,geoPos.y() + 0.029, 0));

        mVisibilityNode = makepolygan(vertices);
        mMapItem->addNode(mVisibilityNode);

        //Set view point------------------------------------------------------------------
//        mMapItem->goToPosition(geoPos.x(), geoPos.y(), 100000);
    }

    if(event->type() ==  QEvent::Type::MouseMove)
    {
//        mIconNode->setText(geoPos.toString());
        mIconNode->setPosition(geoPos);
    }
}

osgEarth::Annotation::FeatureNode* Visibility::makepolygan(QVector<osg::Vec3d> vertices){

    osg::ref_ptr<osgEarth::Symbology::Geometry> geom = new osgEarth::Symbology::Polygon();
    for (int i=0; i<vertices.length();i++){
        geom->push_back(vertices[i]);
    }

    osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(geom, mMapItem->getMapSRS());
    feature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style geomStyle;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Green;
    geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 5.0f;
    geomStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =osgEarth::Color(osg::Vec4(0.0f,1.0f,0.0f,0.5f));
    //geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize()->set(75000, Units::METERS);
    geomStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    osg::ref_ptr<osgEarth::Annotation::FeatureNode> gnode = new osgEarth::Annotation::FeatureNode(feature, geomStyle);
    return  gnode.release();

}

osgEarth::Annotation::ModelNode *Visibility::makeBackground(float radius)
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
    model = new osgEarth::Annotation::ModelNode(mMapItem->getMapNode(), style);

    return model.release();
}

osgEarth::Annotation::PlaceNode *Visibility::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/visibility.png");
    icon->scaleImage(32, 32, icon->r());
//    osg::ref_ptr<osg::Geometry> imageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(icon, osg::Vec2s(0,0), 0, 0, 1);
//    osg::ref_ptr<osg::Geode>  geode = new osg::Geode();
//    geode->addDrawable(imageDrawable);
//    osgEarth::Symbology::Style  style;
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(geode);
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
