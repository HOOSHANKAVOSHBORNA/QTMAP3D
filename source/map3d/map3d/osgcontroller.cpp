
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>

#include "osgcontroller.h"


OsgController::OsgController(QQuickWindow *window) :
    mpWindow(window)
{

}

OsgController::~OsgController()
{

}

void OsgController::cleanup()
{
    if (mpOsgRenderer) {
        mpOsgRenderer->deleteLater();
        mpOsgRenderer = nullptr;
    }
}

void OsgController::initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId)
{

    mrenderTargetId = renderTargetId;

    createOsgRenderer(width, height, screen);

    if(mpWindow) {
        QObject::connect(mpOsgRenderer, &OSGRenderer::needsUpdate,
                         mpWindow, &QQuickWindow::update,
                         Qt::DirectConnection);
    }
}

void OsgController::resizeGL(int width, int height, QScreen *screen)
{
    if (mpOsgRenderer) {
        const float pixelRatio = static_cast<float>(screen->devicePixelRatio());
        mpOsgRenderer->resize(0, 0, width, height, pixelRatio);
    }
}

void OsgController::paintGL()
{
    if (mpOsgRenderer) {
        if (misFirstFrame) {
            misFirstFrame = false;
            mpOsgRenderer->getCamera()->getGraphicsContext()->setDefaultFboId(mrenderTargetId);
        }

        mpOsgRenderer->frame();
    }
}

void OsgController::keyPressEvent(QKeyEvent *event)
{
    if (mpOsgRenderer)
        mpOsgRenderer->keyPressEvent(event);
}

void OsgController::keyReleaseEvent(QKeyEvent *event)
{

    if (mpOsgRenderer)
        mpOsgRenderer->keyReleaseEvent(event);
}

void OsgController::mousePressEvent(QMouseEvent *event)
{
    if (mpOsgRenderer) {
        mpOsgRenderer->mousePressEvent(event);
    }

}

void OsgController::mouseReleaseEvent(QMouseEvent *event)
{

    if (mpOsgRenderer) {
        mpOsgRenderer->mouseReleaseEvent(event);
    }
}

void OsgController::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mpOsgRenderer)
        mpOsgRenderer->mouseDoubleClickEvent(event);

}

void OsgController::mouseMoveEvent(QMouseEvent *event)
{

    if (mpOsgRenderer) {
        mpOsgRenderer->mouseMoveEvent(event);
    }
}

void OsgController::wheelEvent(QWheelEvent *event)
{

    if (mpOsgRenderer)
        mpOsgRenderer->wheelEvent(event);
}


void OsgController::createOsgRenderer(int width, int height, QScreen *screen)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNvOptimusEnablement(1);
    ds->setStereo(false);


    mpOsgRenderer = new OSGRenderer(this);
    const float pixelRatio = static_cast<float>(screen->devicePixelRatio());
    mpOsgRenderer->setupOSG(0 , 0, width, height, pixelRatio);

    mpOsgRenderer->getCamera()->setClearColor(osg::Vec4(0.15f, 0.15f, 0.15f, 1.0f));

}

void OsgController::initializeOsgEarth()
{
    createCameraManipulator();
    mpOsgRenderer->setCameraManipulator(mpEarthManipulator);

    installEventHandler();

    mMapRoot = new osg::Group();
    mSkyNode = osgEarth::Util::SkyNode::create(mMapNode);
    createMapNode(true);

    mpOsgRenderer->setSceneData(mMapRoot);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    mMapNode->getMap()->addLayer(imlayer);
}

void OsgController::createMapNode(bool bGeocentric)
{
    mSkyNode->removeChild(mMapNode);
    mMapRoot->removeChild(mMapNode);
    mMapRoot->removeChild(mSkyNode);

    osgEarth::MapOptions mapOpt;
    if(bGeocentric)
    {
        mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_GEOCENTRIC;
        mapOpt.profile() = osgEarth::ProfileOptions("global-mercator");
        mMapNode = new osgEarth::MapNode(new osgEarth::Map(mapOpt));
    }
    else
    {
        mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;
        mapOpt.profile() = osgEarth::ProfileOptions();
        mMapNode = new osgEarth::MapNode(new osgEarth::Map(mapOpt));
    }

    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
    mMapRoot->addChild(mMapNode);
}

void OsgController::createCameraManipulator()
{
    mpEarthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = mpEarthManipulator->getSettings();
    settings->setSingleAxisRotation(true);

    settings->setMinMaxDistance(10.0, 1000000000.0);
    settings->setMaxOffset(5000.0, 5000.0);
    settings->setMinMaxPitch(-90, 90);
    settings->setTerrainAvoidanceEnabled(true);
    settings->setThrowingEnabled(false);

}
