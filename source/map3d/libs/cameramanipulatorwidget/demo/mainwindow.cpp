#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cameramanipulatorwidget.h"
#include "campasswidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
      CameraManipulatorWidget *view = new CameraManipulatorWidget();
      ui->dockWidget_2->setWidget(view);
    view->setZoomStep(0.02);
    view->setUpDownStep(0.04);
    view->setLeftRightStep(0.05);
    view->setPitchStep(10);
    view->setHeadStep(0.003);
    connect(view,&CameraManipulatorWidget::headChanged,[](double val){
        qDebug()<<val;
    });
    connect(view,&CameraManipulatorWidget::mapChange,[](bool val){
        qDebug()<<val;
    });

    CampassWidget *campass = new CampassWidget();
    //campass->setRotate(55.9);
    ui->horizontalLayout->addWidget(campass);
    connect(view,&CameraManipulatorWidget::pitchChanged,campass,&CampassWidget::setRotate);
    //connect(view,SIGNAL(pitchChanged(double *)),campass,SLOT(setRotate(double *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

