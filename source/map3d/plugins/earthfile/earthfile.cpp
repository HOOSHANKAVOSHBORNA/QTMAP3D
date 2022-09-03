#include "earthfile.h"

#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QDockWidget>

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
          qDebug() << "selected file path : " << fileName.toUtf8();
//          auto map3dWidget = new Map3dWidget(QString(fileName.toUtf8()), this);
//          setCentralWidget(map3dWidget);
        }
    });
    docWidget->setWidget(pb);
}
