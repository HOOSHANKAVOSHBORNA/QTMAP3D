#include "earthfile.h"
#include "map3dwidget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QDockWidget>

#include <osgDB/FileUtils>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>

EarthFile::EarthFile(QWidget *parent):
    PluginInterface(parent)
{

}

void EarthFile::setUpUI()
{
    auto pb = new QPushButton();
    pb->setText("load earth file");
    QObject::connect(pb, &QPushButton::clicked, [=]{
        auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "../map3dlib/data", tr("Image Files (*.earth *.tif *.bmp)"));
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
          osg::ref_ptr<osgEarth::MapNode> mapNode = osgEarth::MapNode::get(baseMap);
          map3dWidget->setMap(mapNode->getMap());

//          auto map3dWidget = new Map3dWidget(QString(fileName.toUtf8()), this);
//          setCentralWidget(map3dWidget);
        }
    });
    docWidget->setWidget(pb);
}
