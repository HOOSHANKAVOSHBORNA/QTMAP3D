#include "featurelayer.h"
#include "MultiChooseDlg.h"

#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "urldialog.h"
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

FeatureLayer::FeatureLayer(QWidget *parent)
    : PluginInterface(parent)
{
}

void FeatureLayer::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Feature;
    QString nameGDAL = "GDAL(local file)";
    mToolBar->addItem(cat, nameGDAL, "", false);

    QString nameArcGIS = "ArcGis";
    mToolBar->addItem(cat, nameArcGIS, "", false);

    QString nameWFS = "WFS";
    mToolBar->addItem(cat, nameWFS, "", false);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool /*isCheck*/){
        if(cat == category && name == nameGDAL)
        {
            addGDAL();
        }
        if(cat == category && name == nameArcGIS)
        {
            addArcGIS();
        }
        if(cat == category && name == nameWFS)
        {
            addWFS();
        }

    });
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
        mMap3dWidget->addLayer(layer);

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
         mMap3dWidget->addLayer(layer);

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

        mMap3dWidget->addLayer(layer);
    }
}


