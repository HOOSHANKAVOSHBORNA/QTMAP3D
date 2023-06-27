#include "drawImage.h"
#include "imageProperties.h"
#include <QFileDialog>


#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"
//#include "osg/Group"
#include "osgEarth/ModelLayer"
#include "osg/Group"
#include "osgEarth/Layer"
#include <QQuickItem>
#include <osg/Image>


int DrawImage::mCount{0};

DrawImage::DrawImage(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<ImagePropertiesModel>("Crystal", 1, 0, "ImageProperties");
}

bool DrawImage::setup()
{
    auto toolboxItem =  new ToolboxItem{IMGOVERLAY, CATEGORY, "qrc:/resources/image.png", true,};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawImage::onImageItemCheck);
    toolbox()->addItem(toolboxItem);
    makeIconNode("../data/images/draw/image.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mImgLayer = new osgEarth::Annotation::AnnotationLayer();
    mImgLayer->setName(IMGOVERLAY);

    return true;
}

void DrawImage::loadImage()
{
    QUrl fileDialog = QFileDialog::getOpenFileUrl();
    imageAddr = osgDB::readImageFile(fileDialog.toLocalFile().toStdString());
}

void DrawImage::onImageItemCheck(bool check)
{
    if (check) {
        loadImage();
        mapItem()->addLayerToLayer(mImgLayer, CATEGORY);
        setState(State::READY);
        mImageProperties = new ImageProperties(mImageOverlay, qmlEngine(), uiHandle(), mapItem());
        mImageProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mImgLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mImgLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mImageOverlay = nullptr;
        mImageProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawImage::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "ImgOverlay" + QString::number(mCount);
    mImageOverlay = new osgEarth::Annotation::ImageOverlay(mapItem()->getMapNode(), imageAddr);
    mImageOverlay->setName(name.toStdString());

    mImageOverlay->setCenter(geoPos.x(),geoPos.y());

    mapItem()->addNodeToLayer(mImageOverlay, IMGOVERLAY);
    mImageProperties->setImage(mImageOverlay);

    mImageProperties->setLocation(mImageOverlay->getCenter());
    mImageProperties->setTL(mImageOverlay->getUpperLeft());
    mImageProperties->setTR(mImageOverlay->getUpperRight());
    mImageProperties->setBR(mImageOverlay->getLowerRight());
    mImageProperties->setBL(mImageOverlay->getLowerLeft());

    setState(State::EDIT);
    mCount++;
}

void DrawImage::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mImageOverlay, IMGOVERLAY);
        mImageOverlay = nullptr;
        mImageProperties->setImage(mImageOverlay);
        setState(State::READY);
        mCount--;
    }
}
