#include "earthfile.h"
#include "mapItem.h"

#include <QFileDialog>
#include <QPushButton>
#include <QDebug>

#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>

const QString CATEGORY = "File";
const QString IMPORT = "Import(.earth)";
const QString EXPORT = "Export(.earth)";

EarthFile::EarthFile(QWidget *parent):
    PluginInterface(parent)
{

}
//bool EarthFile::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
//    Q_UNUSED(engine)
//    desc->fileItemsList.push_back(new ItemDesc{IMPORT, CATEGORY, "qrc:/resources/import.png", false});
//    desc->fileItemsList.push_back(new ItemDesc{EXPORT, CATEGORY, "qrc:/resources/export.png", false});

//    return true;
//}

//void EarthFile::onFileItemClicked(const QString &name, const QString &category)
//{
//    if(CATEGORY == category && name == IMPORT)
//    {
//        auto fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Earth File"), "../data", tr("Earth File (*.earth)"),nullptr,QFileDialog::DontUseNativeDialog);
//        if( !fileName.isNull() )
//        {
//          //qDebug() << "selected file path : " << fileName.toUtf8();
//          //create map node---------------------------------------------

//          osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
//          osgEarth::Config c;
//          c.add("elevation_smoothing", false);
//          osgEarth::TerrainOptions to(c);
//          osgEarth::MapNodeOptions defMNO;
//          defMNO.setTerrainOptions(to);
//          myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

//          osg::ref_ptr<osg::Node> baseMap = osgDB::readNodeFile(fileName.toStdString(), myReadOptions);
//          osg::ref_ptr<osgEarth::MapNode> mapNode1 = osgEarth::MapNode::get(baseMap);
//          mapItem()->setMap(mapNode1->getMap());

//        }
//    }
//    else if(CATEGORY == category && name == EXPORT)
//    {
//        auto fileName = QFileDialog::getSaveFileName(nullptr, tr("Save Earth File"), "../data", tr("Earth File (*.earth)"));
//        if( !fileName.isNull() )
//        {
//            osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
//            osgEarth::Config c;
//            c.add("elevation_smoothing", false);
//            osgEarth::TerrainOptions to(c);
//            osgEarth::MapNodeOptions defMNO;
//            defMNO.setTerrainOptions(to);
//            myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

//            osgDB::writeNodeFile(*mapItem()->getMapNode(), fileName.toStdString(), myReadOptions);
//        }
//    }
//}

bool EarthFile::setup()
{
    auto toolboxItemImport =  new ToolboxItem{IMPORT, CATEGORY, "qrc:/resources/import.png", false};
    QObject::connect(toolboxItemImport, &ToolboxItem::itemClicked, this, &EarthFile::onImportItemClick);
    toolbox()->addItem(toolboxItemImport);

    auto toolboxItemExport =  new ToolboxItem{EXPORT, CATEGORY, "qrc:/resources/export.png", false};
    QObject::connect(toolboxItemExport, &ToolboxItem::itemClicked, this, &EarthFile::onExportItemClick);
    toolbox()->addItem(toolboxItemExport);

    return true;
}

void EarthFile::onImportItemClick()
{
    auto fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Earth File"), "../data", tr("Earth File (*.earth)"),nullptr,QFileDialog::DontUseNativeDialog);
    if( !fileName.isNull() )
    {
        //qDebug() << "selected file path : " << fileName.toUtf8();
        //create map node---------------------------------------------

        osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
        osgEarth::Config c;
        c.add("elevation_smoothing", false);
        osgEarth::TerrainOptions to(c);
        osgEarth::MapNodeOptions defMNO;
        defMNO.setTerrainOptions(to);
        myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

        osg::ref_ptr<osg::Node> baseMap = osgDB::readNodeFile(fileName.toStdString(), myReadOptions);
        osg::ref_ptr<osgEarth::MapNode> mapNode1 = osgEarth::MapNode::get(baseMap);
        mapItem()->setMap(mapNode1->getMap());

    }
}

void EarthFile::onExportItemClick()
{
    auto fileName = QFileDialog::getSaveFileName(nullptr, tr("Save Earth File"), "../data", tr("Earth File (*.earth)"));
    if( !fileName.isNull() )
    {
        osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
        osgEarth::Config c;
        c.add("elevation_smoothing", false);
        osgEarth::TerrainOptions to(c);
        osgEarth::MapNodeOptions defMNO;
        defMNO.setTerrainOptions(to);
        myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

        osgDB::writeNodeFile(*mapItem()->getMapNode(), fileName.toStdString(), myReadOptions);
    }
}
