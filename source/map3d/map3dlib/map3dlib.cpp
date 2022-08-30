#include "map3dlib.h"
#include <QDockWidget>
#include <QFileDialog>
#include <QPushButton>

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }
    QIcon ic(":/res/map3d.ico");
    setWindowIcon(ic);

    auto map3dWidget = new Map3dWidget(true, this);
//    auto map3dWidget = new Map3dWidget(QString("../map3dlib/res/earth_files/projected.earth"), this);
    QDockWidget *docWidget = new QDockWidget(this);
    auto pb = new QPushButton();
    pb->setText("load earth file");
    QObject::connect(pb, &QPushButton::clicked, [=]{
        auto fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "../map3dlib/data", tr("Image Files (*.earth *.tif *.bmp)"));
        if( !fileName.isNull() )
        {
          qDebug() << "selected file path : " << fileName.toUtf8();
          auto map3dWidget = new Map3dWidget(QString(fileName.toUtf8()), this);
          setCentralWidget(map3dWidget);
        }
    });
    docWidget->setWidget(pb);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, docWidget);
    setCentralWidget(map3dWidget);
}

