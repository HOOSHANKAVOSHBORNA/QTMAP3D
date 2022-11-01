#include "terrainlayer.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "urldialog.h"

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/wcs/WCSOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ElevationLayer>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>


TerrainLayer::TerrainLayer(QWidget *parent)
    : PluginInterface(parent)
{
}

void TerrainLayer::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Terrain;
    QString nameGDAL = "GDAL(local file)";
    mToolBar->addItem(cat, nameGDAL, "", false);

    QString nameArcGIS = "Arc GIS";
    mToolBar->addItem(cat, nameArcGIS, "", false);

    QString nameWCS = "WCS";
    mToolBar->addItem(cat, nameWCS, "", false);

    QString nameTMS = "TMS";
    mToolBar->addItem(cat, nameTMS, "", false);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool /*isCheck*/){
        if(cat == category && name == nameGDAL)
        {
            addGDAL();
        }
        if(cat == category && name == nameArcGIS)
        {
            addArcGIS();
        }
        if(cat == category && name == nameWCS)
        {
            addWCS();
        }
        if(cat == category && name == nameTMS)
        {
            addTMS();
        }
    });
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
        mMap3dWidget->addLayer(layer);
    }
}

void TerrainLayer::addArcGIS()
{
    QMap<QString, QString> examples;
    examples[tr("3DEP")] = "https://elevation.nationalmap.gov/arcgis/rest/services/3DEPElevation/ImageServer";
    URLDialog* dialog = new URLDialog(examples,mMainWindow);
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
        mMap3dWidget->addLayer(layer);
    }
}

void TerrainLayer::addWCS()
{
    QMap<QString, QString> examples;
    examples["3DEP"] = "https://elevation.nationalmap.gov/arcgis/services/3DEPElevation/ImageServer/WCSServer?";
    URLDialog* dialog = new URLDialog(examples,mMainWindow);
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
        mMap3dWidget->addLayer(layer);
    }
}

void TerrainLayer::addTMS()
{
    QMap<QString, QString> examples;
    examples[tr("readymap")] = "http://readymap.org/readymap/tiles/1.0.0/116/";
    URLDialog* dialog = new URLDialog(examples,mMainWindow);
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
        mMap3dWidget->addLayer(layer);
    }
}
