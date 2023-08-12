#include "drawImage.h"
#include "imageProperty.h"
#include <QFileDialog>


int DrawImage::mCount{0};

DrawImage::DrawImage(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<ImagePropertyModel>("Crystal", 1, 0, "ImageProperty");
}

bool DrawImage::setup()
{
    auto toolboxItem =  new ToolboxItem{IMAGEOVERLAY, CATEGORY, "qrc:/resources/image.png", true,};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawImage::onImageItemCheck);
    toolbox()->addItem(toolboxItem);
    makeIconNode("../data/images/draw/image.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    mImgLayer = new ParenticAnnotationLayer();
    mImgLayer->setName(IMAGEOVERLAY);
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
        setState(State::READY);
//        mImageProperty = new ImageProperty(mImageOverlay, qmlEngine(), uiHandle(), mapItem());
//        mImageProperty->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
        cancelDraw();
        setState(State::NONE);
        mImageOverlay = nullptr;
//        mImageProperty->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawImage::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "ImageOverlay" + QString::number(mCount);
    mImageOverlay = new osgEarth::Annotation::ImageOverlay(mapItem()->getMapNode(), imageAddr);
    mImageOverlay->setName(name.toStdString());
    mImageOverlay->setCenter(geoPos.x(),geoPos.y());

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mImgLayer)){
        mImgLayer->clear();
        shapeLayer->addLayer(mImgLayer);
    }

//    mImageProperty->setImage(mImageOverlay);
//    mImageProperty->setLocation(mImageOverlay->getCenter());
//    mImageProperty->setTL(mImageOverlay->getUpperLeft());
//    mImageProperty->setTR(mImageOverlay->getUpperRight());
//    mImageProperty->setBR(mImageOverlay->getLowerRight());
//    mImageProperty->setBL(mImageOverlay->getLowerLeft());
    mImgLayer->addChild(mImageOverlay);
    setState(State::DRAWING);
    mCount++;
}

void DrawImage::cancelDraw()
{
    if(state() == State::DRAWING){
        mImgLayer->removeChild(mImageOverlay);
        mImageOverlay = nullptr;
//        mImageProperty->setImage(mImageOverlay);
        setState(State::READY);
        mCount--;

        if(!mImgLayer->hasNode()){
            DrawShape::shapeLayer()->removeLayer(mImgLayer);
        }
    }
}
