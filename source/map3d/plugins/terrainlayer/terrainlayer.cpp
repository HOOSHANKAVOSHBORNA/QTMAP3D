#include "terrainlayer.h"
#include "urldialog.h"
#include "mapcontroller.h"

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/wcs/WCSOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ElevationLayer>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>


const QString CATEGORY = "Terrain";
const QString GDAL = "GDAL(local file)";
const QString ARCGIS = "Arc GIS";
const QString WCS = "WCS";
const QString TMS = "TMS";

TerrainLayer::TerrainLayer(QWidget *parent)
    : PluginInterface(parent)
{
}

bool TerrainLayer::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    Q_UNUSED(engine)
    desc->toolboxItemsList.push_back(new ItemDesc{GDAL, CATEGORY, "qrc:/resources/gdal.png", false, false, ""});
    desc->toolboxItemsList.push_back(new ItemDesc{ARCGIS, CATEGORY, "qrc:/resources/arcgis.png", false, false, ""});
    desc->toolboxItemsList.push_back(new ItemDesc{WCS, CATEGORY, "qrc:/resources/wcs.png", false, false, ""});
    desc->toolboxItemsList.push_back(new ItemDesc{TMS, CATEGORY, "qrc:/resources/tms.png", false, false, ""});

    return true;
}

void TerrainLayer::onToolboxItemClicked(const QString &name, const QString &category)
{
    if(CATEGORY == category && name == GDAL)
    {
        addGDAL();
    }
    if(CATEGORY == category && name == ARCGIS)
    {
        addArcGIS();
    }
    if(CATEGORY == category && name == WCS)
    {
        addWCS();
    }
    if(CATEGORY == category && name == TMS)
    {
        addTMS();
    }
}

bool TerrainLayer::setup(MapController *mapController, NetworkManager *networkManager)
{
    mMapController = mapController;
}

void TerrainLayer::addGDAL()
{
    auto fileNames = QFileDialog::getOpenFileNames(nullptr, tr("Open Terrain File"), "../map3dlib/data",
                                                   tr("Image File (*.img *.tif *.tiff);;Allfile(*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    for (auto fileName : fileNames)
    {
        std::string  nodeName = fileName.toStdString();

        osgEarth::Drivers::GDALOptions  opt;
        opt.url() = nodeName;
        osg::ref_ptr<osgEarth::ElevationLayer>  layer = new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions(nodeName, opt));
        mMapController->addLayer(layer);
    }
}

void TerrainLayer::addArcGIS()
{
    QMap<QString, QString> examples;
    examples[tr("3DEP")] = "https://elevation.nationalmap.gov/arcgis/rest/services/3DEPElevation/ImageServer";
    URLDialog* dialog = new URLDialog(examples,nullptr);
    dialog->setWindowTitle("Select ArcGIS url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        auto nodeName = url.toStdString();
        osgEarth::Drivers::ArcGISOptions opt;
        opt.url() = nodeName;

        osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions(nodeName, opt));
        mMapController->addLayer(layer);
    }
}

void TerrainLayer::addWCS()
{
    QMap<QString, QString> examples;
    examples["3DEP"] = "https://elevation.nationalmap.gov/arcgis/services/3DEPElevation/ImageServer/WCSServer?";
    URLDialog* dialog = new URLDialog(examples,nullptr);
    dialog->setWindowTitle("Select WCS url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        auto nodeName = url.toStdString();
        osgEarth::Drivers::WCSOptions  opt;
        opt.url()        = nodeName;
        opt.format()     = "image/GeoTIFF";
        opt.profile()    = { "EPSG:3857" };
        opt.identifier() = "DEP3ElevationPrototype";

        osg::ref_ptr<osgEarth::ElevationLayer>  layer = new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions(nodeName, opt));
        mMapController->addLayer(layer);
    }
}

void TerrainLayer::addTMS()
{
    QMap<QString, QString> examples;
    examples[tr("readymap")] = "http://readymap.org/readymap/tiles/1.0.0/116/";
    URLDialog* dialog = new URLDialog(examples,nullptr);
    dialog->setWindowTitle("Select TMS url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        auto nodeName = url.toStdString();
        osgEarth::Drivers::TMSOptions opt;
        opt.url() = nodeName;
        osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions(nodeName, opt));
        mMapController->addLayer(layer);
    }
}
