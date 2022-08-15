#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cameramanipulatorwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CameraManipulatorWidget *view = new CameraManipulatorWidget();
    ui->dockWidget->setWidget(view);
    view->setZoomStep(0.02);
    view->setUpDownStep(0.04);
    view->setLeftRightStep(0.05);
    view->setPitchStep(0.86);
    view->setHeadStep(0.003);
    connect(view,&CameraManipulatorWidget::headChanged,[](double val){
        qDebug()<<val;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

