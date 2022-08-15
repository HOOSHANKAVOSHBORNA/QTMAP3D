#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cameramanipulatorwidget.h"
#include "getdata.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CameraManipulatorWidget *view = new CameraManipulatorWidget();
    ui->dockWidget->setWidget(view);
    view->setZoomStep(3);
    view->setUpDownStep(4);
    view->setLeftRightStep(5);
    view->setPitchStep(6);
    view->setHeadStep(7);
    connect(view,&CameraManipulatorWidget::headChanged,[](int val){
        qDebug()<<val;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

