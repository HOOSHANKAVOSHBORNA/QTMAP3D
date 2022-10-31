#include "terrainlayer.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "urldialog.h"

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
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

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool /*isCheck*/){
        if(cat == category && name == nameGDAL)
        {
            addGDAL();
        }
        if(cat == category && name == nameArcGIS)
        {
            addArcGIS();
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
    URLDialog* dialog = new URLDialog(examples,this);
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
