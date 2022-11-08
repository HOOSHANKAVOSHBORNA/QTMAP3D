#include "visibility.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "websocketclient.h"

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osg/Shader>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <QDebug>
Visibility::Visibility(QWidget *parent)
    : PluginInterface(parent)
{
}

void Visibility::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Analyze;
    QString nameVisibility = "Visibility";
    mToolBar->addItem(cat, nameVisibility, "qrc:/res/visibility.png",true);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool isCheck){
        if(cat == category && name == nameVisibility)
        {
            createWebSocket();
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &Visibility::onMouseEvent);


            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &Visibility::onMouseEvent);

                mMap3dWidget->removeNode(mBackVisibilityNode);
                mMap3dWidget->removeNode(mVisibilityNode);
            }
        }
    });
}

void Visibility::onMouseEvent(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {

        mMap3dWidget->removeNode(mBackVisibilityNode);
        mMap3dWidget->removeNode(mVisibilityNode);

        geoPos.makeGeographic();
        mWebSocket->sendTextMessage(QString::fromStdString(geoPos.toString()));

        mBackVisibilityNode = makeBackground(20000.0f);
        //osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.461);
        mBackVisibilityNode->setPosition(geoPos);
        mMap3dWidget->addNode(mBackVisibilityNode);
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
        mMap3dWidget->addNode(mVisibilityNode);

        //Set view point------------------------------------------------------------------
        mMap3dWidget->goPosition(geoPos.x(), geoPos.y(), 100000);
    }
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
    model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);

    return model.release();
}
