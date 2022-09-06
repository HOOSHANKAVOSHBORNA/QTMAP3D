#include "earthfile.h"
#include "map3dwidget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QAction>
#include <QToolBar>

#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>

EarthFile::EarthFile(QWidget *parent):
    PluginInterface(parent)
{

}

void EarthFile::setUpUI()
{
    const QIcon readIcon (":/res/read.png");
    auto readAction = new QAction(readIcon, "Import .earth");
    readAction->setToolTip("Import map from earth file.");
    QObject::connect(readAction, &QAction::triggered, [=]{
        auto fileName = QFileDialog::getOpenFileName(this, tr("Open Earth File"), "../map3dlib/data", tr("Earth File (*.earth)"));
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
          mMap3dWidget->setMap(mapNode1->getMap());

        }
    });
    mToolBar->addAction(readAction);
    //------------------------------------------------------------
    const QIcon writeIcon (":/res/write.png");
    auto writeAction = new QAction(writeIcon, "Export .earth");
    writeAction->setToolTip("Export map to earth file.");
    QObject::connect(writeAction, &QAction::triggered, [=]{
        auto fileName = QFileDialog::getSaveFileName(this, tr("Save Earth File"), "../map3dlib/data", tr("Earth File (*.earth)"));
        if( !fileName.isNull() )
        {
            osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
            osgEarth::Config c;
            c.add("elevation_smoothing", false);
            osgEarth::TerrainOptions to(c);
            osgEarth::MapNodeOptions defMNO;
            defMNO.setTerrainOptions(to);
            myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

            osgDB::writeNodeFile(*mMap3dWidget->getMapNode(), fileName.toStdString(), myReadOptions);
        }
    });
    mToolBar->addAction(writeAction);
}
