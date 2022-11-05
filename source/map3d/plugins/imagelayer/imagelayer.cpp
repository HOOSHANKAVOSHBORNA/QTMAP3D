#include "imagelayer.h"
#include "MultiChooseDlg.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "urldialog.h"

#include <osgEarth/Registry>
#include <osgEarth/XmlUtils>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/wms/WMSOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarth/ImageLayer>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>

ImageLayer::ImageLayer(QWidget *parent)
    : PluginInterface(parent)
{
}

void ImageLayer::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Image;
    QString nameGDAL = "GDAL(local file)";
    mToolBar->addItem(cat, nameGDAL, "", false);

    QString nameArcGIS = "Arc Gis";
    mToolBar->addItem(cat, nameArcGIS, "", false);

    QString nameWMS = "WMS";
    mToolBar->addItem(cat, nameWMS, "", false);

    QString nameTMS = "TMS";
    mToolBar->addItem(cat, nameTMS, "", false);

    QString nameXYZ = "XYZ";
    mToolBar->addItem(cat, nameXYZ, "", false);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool /*isCheck*/){
        if(cat == category && name == nameGDAL)
        {
            GDAL();
        }
        if(cat == category && name == nameArcGIS)
        {
            ArcGis();
        }
        if(cat == category && name == nameXYZ)
        {
            XYZ();
        }
        if(cat == category && name == nameWMS)
        {
            WMS();
        }
        if(cat == category && name == nameTMS)
        {
            TMS();
        }
    });
}

void ImageLayer::XYZ()
{
    QMap<QString, QString> examples;
    examples[tr("Map Box")] = "http://a.tiles.mapbox.com/v4/mapbox.satellite/{z}/{x}/{y}.jpg?access_token=YOUR_TOKEN_HERE";
    examples[tr("Open Street Map")] = "http://[abc].tile.openstreetmap.org/{z}/{x}/{y}.png";
    examples[tr("Gaode")] = "http://wprd0[1234].is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}";
    examples[tr("OpenWeatherMap")] = "http://[abc].tile.openweathermap.org/map/clouds/{z}/{x}/{y}.png";
    URLDialog* dialog = new URLDialog(examples,mMainWindow);
    dialog->setWindowTitle("Select XYZ url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        std::string nodeName = url.toLocal8Bit().toStdString();
        osgEarth::Drivers::XYZOptions opt;
        opt.url() = nodeName;
        opt.profile() = { "spherical-mercator" };

        osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mMap3dWidget->addLayer(layer);

}
}
void ImageLayer::ArcGis()
{
    QMap<QString, QString> examples;
    examples[tr("ArcGIS Online - world")] = "http://services.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer";
    examples[tr("ArcGIS Online - terrain")] = "http://services.arcgisonline.com/arcgis/rest/services/World_Terrain_Base/MapServer";
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

        osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mMap3dWidget->addLayer(layer);
    }
}

void ImageLayer::GDAL()
{
    auto fileNames = QFileDialog::getOpenFileNames(nullptr, tr("Open Image File"), "../map3dlib/data",
                                                   tr("Image File (*.img *.tif *.tiff);;Allfile(*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    for (auto fileName : fileNames)
    {
        std::string  nodeName = fileName.toStdString();

        osgEarth::Drivers::GDALOptions  opt;
        opt.url() = nodeName;
        osg::ref_ptr<osgEarth::ImageLayer>  layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mMap3dWidget->addLayer(layer);
    }
}

void ImageLayer::TMS()
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
        osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mMap3dWidget->addLayer(layer);
    }
}


typedef QPair<QString, QString> attrib;
static QVector<attrib>  getWMSInfo(std::string &path, osgEarth::GeoExtent * &extent)
{
    QVector<attrib>              attribList;
    char                         str[1000];
    osgEarth::URI                          uri(path);
    osg::ref_ptr<const osgEarth::Profile>  result;
    char                         sep    = uri.full().find_first_of('?') == std::string::npos ? '?' : '&';
    osgEarth::URI                          capUrl = osgEarth::URI(
                uri.full()
                + sep
                + std::string("service=WMS")
                + std::string("&REQUEST=GetCapabilities"));
    osgEarth::XmlDocument *doc = osgEarth::XmlDocument::load(capUrl.full());

    if (!doc)
    {
        return attribList;
    }

    auto         capabilities = doc->getSubElement("WMS_Capabilities")->getSubElement("Capability");
    auto         rootLayer    = capabilities->getSubElement("Layer");
    osgEarth::XmlNodeList  layers       = rootLayer->getSubElements("Layer");
    std::string  layersStr;

    for (auto layer : layers)
    {
        auto  layerInfo = static_cast<osgEarth::XmlElement *>(layer.get());
        auto  title     = layerInfo->getSubElement("Name");

        if (!title)
        {
            title = layerInfo->getSubElement("name");
        }

        if (!title)
        {
            title = layerInfo->getSubElement("Title");
        }

        if (!title)
        {
            title = layerInfo->getSubElement("title");
        }

        if (title)
        {
            layersStr += title->getText() + ',';
        }

        double  minX, minY, maxX, maxY;
        auto    e_bb = layerInfo->getSubElement("latlonboundingbox");

        if (e_bb)
        {
            minX = osgEarth::as<double>(e_bb->getAttr("minx"), 0);
            minY = osgEarth::as<double>(e_bb->getAttr("miny"), 0);
            maxX = osgEarth::as<double>(e_bb->getAttr("maxx"), 0);
            maxY = osgEarth::as<double>(e_bb->getAttr("maxy"), 0);
        }
        else
        {
            auto  e_gbb = layerInfo->getSubElement("ex_geographicboundingbox");

            if (e_gbb)
            {
                minX = osgEarth::as<double>(e_gbb->getSubElementText("westBoundLongitude"), 0);
                minY = osgEarth::as<double>(e_gbb->getSubElementText("southBoundLatitude"), 0);
                maxX = osgEarth::as<double>(e_gbb->getSubElementText("eastBoundLongitude"), 0);
                maxY = osgEarth::as<double>(e_gbb->getSubElementText("northBoundLatitude"), 0);
            }
        }

        osgEarth::GeoExtent  layerExtent(osgEarth::Registry::instance()->getGlobalGeodeticProfile()->getSRS(), minX, minY, maxX, maxY);

        if (!extent)
        {
            extent = new osgEarth::GeoExtent(layerExtent);
        }
        else
        {
            extent->expandToInclude(layerExtent);
        }
    }

    sprintf(str, "(%.3lf, %.3lf)", extent->xMin(), extent->yMin());
    attribList.push_back(attrib("min", str));
    sprintf(str, "(%.3lf, %.3lf)", extent->xMax(), extent->yMax());
    attribList.push_back(attrib("max", str));

    layersStr.pop_back();
    attribList.push_back(attrib("layers", layersStr.c_str()));

    return attribList;
}



void ImageLayer::WMS()
{
    QMap<QString, QString> examples;
    examples[tr("NEXRAD")] = "http://mesonet.agron.iastate.edu/cgi-bin/wms/nexrad/n0r.cgi";
    URLDialog* dialog = new URLDialog(examples,mMainWindow);
    dialog->setWindowTitle("Select WMS url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        auto nodeName = url.toLocal8Bit().toStdString();
        osgEarth::GeoExtent *extent   = nullptr;
        auto  attribute = getWMSInfo(nodeName, extent);

        // Promt for the users to choose layers
        QStringList     layerNames = attribute.back().second.split(',');
        MultiChooseDlg  chooseDlg(static_cast<QWidget *>(parent()), layerNames);
        chooseDlg.exec();
        QStringList  layersToShow = chooseDlg.getCheckedItems();
        osgEarth::Drivers::WMSOptions  opt;
        opt.url()         = nodeName;
        opt.transparent() = true;
        opt.format()      = "png";
        opt.profile()     = { "EPSG:4326" };

        osg::ref_ptr<osgEarth::ImageLayer>  layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mMap3dWidget->addLayer(layer);
    }
}


