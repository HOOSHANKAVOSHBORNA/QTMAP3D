#include "imagelayer.h"
#include "multichoosedlg.h"
#include "urldialog.h"
#include "mapItem.h"

#include <osgEarth/Registry>
#include <osgEarth/XmlUtils>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/wms/WMSOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarth/ImageLayer>
#include <osgEarth/ModelLayer>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>

const QString CATEGORY = "Image";
const QString GDAL = "GDAL(local file)";
const QString ARCGIS = "Arc GIS";
const QString WMS = "WMS";
const QString TMS = "TMS";
const QString XYZ = "XYZ";

ImageLayer::ImageLayer(QWidget *parent)
    : PluginInterface(parent)
{
}
//bool ImageLayer::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
//    Q_UNUSED(engine)
//    desc->toolboxItemsList.push_back(new ItemDesc{GDAL, CATEGORY, "qrc:/resources/gdal.png", false});
//    desc->toolboxItemsList.push_back(new ItemDesc{ARCGIS, CATEGORY, "qrc:/resources/arcgis.png", false});
//    desc->toolboxItemsList.push_back(new ItemDesc{WMS, CATEGORY, "qrc:/resources/wms.png", false, false, ""});
//    desc->toolboxItemsList.push_back(new ItemDesc{TMS, CATEGORY, "qrc:/resources/tms.png", false, false, ""});
//    desc->toolboxItemsList.push_back(new ItemDesc{XYZ, CATEGORY, "qrc:/resources/xyz.png", false, false, ""});

//    return true;
//}

//void ImageLayer::onToolboxItemClicked(const QString &name, const QString &category)
//{
//    if(CATEGORY == category && name == GDAL)
//    {
//        addGDAL();
//    }
//    if(CATEGORY == category && name == ARCGIS)
//    {
//        addArcGIS();
//    }
//    if(CATEGORY == category && name == WMS)
//    {
//        addWMS();
//    }
//    if(CATEGORY == category && name == TMS)
//    {
//        addTMS();
//    }
//    if(CATEGORY == category && name == XYZ)
//    {
//        addXYZ();
//    }
//}

bool ImageLayer::setup()
{
    auto toolboxItemGDAL =  new ToolboxItem{GDAL, CATEGORY, "qrc:/resources/gdal.png", false};
    QObject::connect(toolboxItemGDAL, &ToolboxItem::itemClicked, this, &ImageLayer::addGDAL);
    toolbox()->addItem(toolboxItemGDAL);

    auto toolboxItemArcGIS =  new ToolboxItem{ARCGIS, CATEGORY, "qrc:/resources/arcgis.png", false};
    QObject::connect(toolboxItemArcGIS, &ToolboxItem::itemClicked, this, &ImageLayer::addArcGIS);
    toolbox()->addItem(toolboxItemArcGIS);

    auto toolboxItemWMS =  new ToolboxItem{WMS, CATEGORY, "qrc:/resources/wms.png", false};
    QObject::connect(toolboxItemWMS, &ToolboxItem::itemClicked, this, &ImageLayer::addWMS);
    toolbox()->addItem(toolboxItemWMS);

    auto toolboxItemTMS =  new ToolboxItem{TMS, CATEGORY, "qrc:/resources/tms.png", false};
    QObject::connect(toolboxItemTMS, &ToolboxItem::itemClicked, this, &ImageLayer::addTMS);
    toolbox()->addItem(toolboxItemTMS);

    auto toolboxItemXYZ =  new ToolboxItem{XYZ, CATEGORY, "qrc:/resources/xyz.png", false};
    QObject::connect(toolboxItemXYZ, &ToolboxItem::itemClicked, this, &ImageLayer::addXYZ);
    toolbox()->addItem(toolboxItemXYZ);


    return true;
}

void ImageLayer::addXYZ()
{
    QMap<QString, QString> examples;
    examples[tr("Google R")] = "https://mt1.google.com/vt/lyrs=r&x={x}&y={y}&z={z}";
    examples[tr("Google S")] = "https://mt1.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    examples[tr("Gitanegaran")] = "https://api.gitanegaran.ir/sat/{z}/{x}/{y}.jpg";
    examples[tr("Open Street Map")] = "http://[abc].tile.openstreetmap.org/{z}/{x}/{y}.png";
    examples[tr("Gaode")] = "http://wprd0[1234].is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}";
    examples[tr("OpenWeatherMap")] = "http://[abc].tile.openweathermap.org/map/clouds/{z}/{x}/{y}.png";
    examples[tr("Map Box")] = "http://a.tiles.mapbox.com/v4/mapbox.satellite/{z}/{x}/{y}.jpg?access_token=YOUR_TOKEN_HERE";

    examples[tr("Carto CDN")] = "https://a.basemaps.cartocdn.com/dark_nolabels/{z}/{x}/{y}.png";

    URLDialog* dialog = new URLDialog(examples,nullptr);
    dialog->setWindowTitle("Select XYZ url");

    int accepted = dialog->exec();
    if (accepted == QDialog::Accepted)
    {
        QString url = dialog->getURL();
        if (url.isEmpty())
            return;

        auto it = std::find_if(examples.keyBegin(),
                               examples.keyEnd(),
                               [&examples, &url](const QString& item){

                return examples[item] == url;
        });

        if (it != examples.keyEnd()) {
            std::string nodeName = url.toLocal8Bit().toStdString();
            osgEarth::Drivers::XYZOptions opt;
            opt.url() = nodeName;
            opt.profile() = { "spherical-mercator" };
            auto imageLayerOptions = osgEarth::ImageLayerOptions(nodeName, opt);
            osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
            layer->setName(it->toStdString());
            mapItem()->getMapObject()->addLayer(layer);

        }

    }
}
void ImageLayer::addArcGIS()
{
    QMap<QString, QString> examples;
    examples[tr("ArcGIS Online - world")] = "http://services.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer";
    examples[tr("ArcGIS Online - terrain")] = "http://services.arcgisonline.com/arcgis/rest/services/World_Terrain_Base/MapServer";
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

        osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mapItem()->getMapObject()->addLayer(layer);
    }
}

void ImageLayer::addGDAL()
{
    auto fileNames = QFileDialog::getOpenFileNames(nullptr, tr("Open Image File"), "../map3dlib/data",
                                                   tr("Image File (*.img *.tif *.tiff);;Allfile(*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    for (auto fileName : fileNames)
    {
        std::string  nodeName = fileName.toStdString();

        osgEarth::Drivers::GDALOptions  opt;
        opt.url() = nodeName;
        osg::ref_ptr<osgEarth::ImageLayer>  layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mapItem()->getMapObject()->addLayer(layer);
    }
}

void ImageLayer::addTMS()
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
        osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
        mapItem()->getMapObject()->addLayer(layer);
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



void ImageLayer::addWMS()
{
    QMap<QString, QString> examples;
    examples[tr("NEXRAD")] = "http://mesonet.agron.iastate.edu/cgi-bin/wms/nexrad/n0r.cgi";
    URLDialog* dialog = new URLDialog(examples,nullptr);
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
        //        chooseDlg.exec();

        int acceptedd = chooseDlg.exec();
        if (acceptedd == MultiChooseDlg::Accepted)
        {
            QStringList  layersToShow = chooseDlg.getCheckedItems();

            if (chooseDlg.getCheckedItems().empty())
                return;
            osgEarth::Drivers::WMSOptions  opt;
            opt.url()         = nodeName;
            opt.layers()      = layersToShow.join(',').toLocal8Bit().toStdString();
            opt.transparent() = true;
            opt.format()      = "png";
            opt.profile()     = { "EPSG:4326" };

            osg::ref_ptr<osgEarth::ImageLayer>  layer = new osgEarth::ImageLayer(osgEarth::ImageLayerOptions(nodeName, opt));
            mapItem()->getMapObject()->addLayer(layer);

        }
    }
}


