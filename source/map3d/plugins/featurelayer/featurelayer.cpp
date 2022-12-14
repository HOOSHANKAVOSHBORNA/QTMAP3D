#include "featurelayer.h"
#include "multichoosedlg.h"
#include "urldialog.h"
#include "mapcontroller.h"

#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>

const QString CATEGORY = "Feature";
const QString GDAL = "GDAL(local file)";
const QString ARCGIS = "Arc GIS";
const QString WFS = "WFS";

FeatureLayer::FeatureLayer(QWidget *parent)
    : PluginInterface(parent)
{
}
bool FeatureLayer::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    Q_UNUSED(engine)
    desc->toolboxItemsList.push_back(new ItemDesc{GDAL, CATEGORY, "qrc:/resources/gdal.png", false, false, ""});
    desc->toolboxItemsList.push_back(new ItemDesc{ARCGIS, CATEGORY, "qrc:/resources/arcgis.png", false, false, ""});
    desc->toolboxItemsList.push_back(new ItemDesc{WFS, CATEGORY, "qrc:/resources/wfs.png", false, false, ""});

    return true;
}

void FeatureLayer::onToolboxItemClicked(const QString &name, const QString &category)
{
    if(CATEGORY == category && name == GDAL)
    {
        addGDAL();
    }
    if(CATEGORY == category && name == ARCGIS)
    {
        addArcGIS();
    }
    if(CATEGORY == category && name == WFS)
    {
        addWFS();
    }
}

bool FeatureLayer::setup(MapController *mapController,
                         NetworkManager *networkManager,
                         UIHandle *UIHandle)
{
    mMapController = mapController;
}

void FeatureLayer::addGDAL()
{
    QStringList  fileNames = QFileDialog::getOpenFileNames(dynamic_cast<QWidget *>(parent()), tr("Open File"), "", tr("Tiff File (*.shp);;Allfile(*.*)"));

    if (fileNames.isEmpty())
    {
        return;
    }

    for (auto fileName : fileNames)
    {
        std::string  nodeName = fileName.toLocal8Bit().toStdString();

        osgEarth::Drivers::OGRFeatureOptions  opt;
        opt.url() = nodeName;

        osgEarth::Drivers::FeatureGeomModelOptions  geomOptions;
        geomOptions.clustering()     = false;
        geomOptions.mergeGeometry()  = true;
        geomOptions.featureOptions() = opt;
        //        geomOptions.styles()         = new StyleSheet();
        //        geomOptions.styles()->addStyle(style);
        geomOptions.enableLighting()   = false;
        geomOptions.depthTestEnabled() = false;

        osgEarth::Drivers::ModelLayerOptions *options = new osgEarth::Drivers::ModelLayerOptions(nodeName, geomOptions);
        osg::ref_ptr<osgEarth::ModelLayer>    layer   = new osgEarth::Drivers::ModelLayer(*options);
        mMapController->addLayer(layer);

    }

}

void FeatureLayer::addWFS()
{
    QString  fileName = QInputDialog::getText(
                dynamic_cast<QWidget *>(parent()), tr("Please enter file location"), "");

    if (!fileName.isEmpty())
    {
        std::string                 nodeName = fileName.toLocal8Bit().toStdString();
        QStringList                 featureFieldList;

        MultiChooseDlg  chooseDlg(static_cast<QWidget *>(parent()), featureFieldList);
        chooseDlg.exec();
        QStringList        layersToShow = chooseDlg.getCheckedItems();
        osgEarth::Drivers::WFSFeatureOptions  opt;
        opt.url()          = nodeName;
        opt.outputFormat() = "GML";
        opt.typeName()     = layersToShow.join(',').toLocal8Bit().toStdString();

        osgEarth::Drivers::FeatureGeomModelOptions  geomOptions;
        geomOptions.featureOptions() = opt;
        const osgEarth::optional<osgEarth::Config>  con = opt.getConfig();
        //        geomOptions.styles() = new StyleSheet();
        //        geomOptions.styles()->addStyle(style);
        geomOptions.enableLighting() = false;
        osg::ref_ptr<osgEarth::ModelLayer>  layer = new osgEarth::Drivers::ModelLayer(osgEarth::Drivers::ModelLayerOptions(nodeName, geomOptions));
        mMapController->addLayer(layer);

    }
}

void FeatureLayer::addArcGIS()
{
    QString  fileName = QInputDialog::getText(
                dynamic_cast<QWidget *>(parent()), tr("Please enter file location"), "");

    if (!fileName.isEmpty())
    {
        std::string  nodeName = fileName.toLocal8Bit().toStdString();


        osgEarth::Drivers::ArcGISOptions  opt;
        opt.url() = nodeName;
        osgEarth::Drivers::ArcGISOptions  arcgisonline;
        arcgisonline.url() = osgEarth::URI(nodeName.c_str());
        osgEarth::Drivers::FeatureGeomModelOptions  arcGeomOptions;
        arcGeomOptions.featureOptions() = arcgisonline;
        //        arcGeomOptions.styles()         = new StyleSheet();
        //        arcGeomOptions.styles()->addStyle(style);
        arcGeomOptions.enableLighting() = false;
        osg::ref_ptr<osgEarth::ModelLayer>  layer = new osgEarth::ModelLayer(osgEarth::ModelLayerOptions(nodeName, arcGeomOptions));

        mMapController->addLayer(layer);
    }
}


