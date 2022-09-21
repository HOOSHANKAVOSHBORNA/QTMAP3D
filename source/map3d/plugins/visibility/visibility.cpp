#include "visibility.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osg/Shader>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>

Visibility::Visibility(QWidget *parent)
    : PluginInterface(parent)
{
}

void Visibility::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Analyze;
    QString nameVisibility = "Visibility";
    mToolBar->addItem(cat, nameVisibility, "");

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name){
        if(cat == category && name == nameVisibility)
        {
            osg::Vec3 position(0.3f,0.2f,1.0f);
            osg::ref_ptr<osg::Node> mDrownode = makeShape(osg::Vec3(position.x(),position.y(),position.z()),20000.0f);
            osgEarth::Symbology::Style  style;

            style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
            style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
            style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mDrownode);

            osg::ref_ptr<osgEarth::Annotation::ModelNode>  model;
            model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);
            osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.461);
            model->setPosition(point);
            mMap3dWidget->getMapNode()->addChild(model);
            ///////////
            QVector<osg::Vec3d> vertices;
            vertices.push_back( osg::Vec3d(52.859,35.550, 0) );
            vertices.push_back( osg::Vec3d(52.859,35.540, 0) );
            vertices.push_back( osg::Vec3d(52.865,35.540, 0) );
            vertices.push_back( osg::Vec3d(52.865,35.550, 0) );
            QVector<osg::Vec3d> vertices1;
            vertices1.push_back( osg::Vec3d(52.900,35.500, 0) );
            vertices1.push_back( osg::Vec3d(52.850,35.450, 0) );
            vertices1.push_back( osg::Vec3d(52.900,35.400, 0) );
            vertices1.push_back( osg::Vec3d(52.950,35.400, 0) );
            vertices1.push_back( osg::Vec3d(53.000,35.450, 0) );
            vertices1.push_back( osg::Vec3d(53.050,35.500, 0) );

            QVector<osg::Vec3d> vertices2;
            vertices2.push_back( osg::Vec3d(52.800,35.500, 0) );
            vertices2.push_back( osg::Vec3d(52.770,35.470, 0) );
            vertices2.push_back( osg::Vec3d(52.830,35.440, 0) );
            vertices2.push_back( osg::Vec3d(52.860,35.470, 0) );
            vertices2.push_back( osg::Vec3d(52.860,35.500, 0) );

            QVector<osg::Vec3d> vertices3;
            vertices3.push_back( osg::Vec3d(52.800,35.430, 0) );
            vertices3.push_back( osg::Vec3d(52.760,35.390, 0) );
            vertices3.push_back( osg::Vec3d(52.760,35.350, 0) );
            vertices3.push_back( osg::Vec3d(52.800,35.310, 0) );
            vertices3.push_back( osg::Vec3d(52.840,35.310, 0) );
            vertices3.push_back( osg::Vec3d(52.880,35.350, 0) );
            vertices3.push_back( osg::Vec3d(52.880,35.390, 0) );
            vertices3.push_back( osg::Vec3d(52.840,35.430, 0) );


            osgEarth::Annotation::FeatureNode* fNode =makepolygan(vertices);
            mMap3dWidget->getMapNode()->addChild(fNode);
            osgEarth::Annotation::FeatureNode* fNode1 =makepolygan(vertices1);
            mMap3dWidget->getMapNode()->addChild(fNode1);
            osgEarth::Annotation::FeatureNode* fNode2 =makepolygan(vertices2);
            mMap3dWidget->getMapNode()->addChild(fNode2);
            osgEarth::Annotation::FeatureNode* fNode3 =makepolygan(vertices3);
            mMap3dWidget->getMapNode()->addChild(fNode3);
        }
    });
}

osgEarth::Annotation::FeatureNode* Visibility::makepolygan(QVector<osg::Vec3d> vertices){

    osg::ref_ptr<osgEarth::Symbology::Geometry> geom = new osgEarth::Symbology::Polygon();
    for (int i=0; i<vertices.length();i++){
        geom->push_back(vertices[i]);
    }

    osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(geom, osgEarth::SpatialReference::get("wgs84"));
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

osg::Geode* Visibility::makeShape(osg::Vec3d eye,float radius)
{
    osg::ref_ptr<osg::Sphere>         pSphereShape   = new osg::Sphere(eye, radius);
    osg::ref_ptr<osg::ShapeDrawable>  pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());

    pShapeDrawable->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.3f));

    osg::ref_ptr<osg::Geode>  geode = new osg::Geode();
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);

    geode->addDrawable(pShapeDrawable.get());

    return geode.release();
}
