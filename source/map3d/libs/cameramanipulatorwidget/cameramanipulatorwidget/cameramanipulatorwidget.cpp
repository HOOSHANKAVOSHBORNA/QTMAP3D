#include "cameramanipulatorwidget.h"
CameraManipulatorWidget::CameraManipulatorWidget(QWidget *parent):
    QWidget(parent)
{
    mZoomStep=0.5;
    mUpDownstep=0.5;
    mLeftRightStep=0.5;
    mPitchStep=0.5;
    mHeadStep=0.5;
    mView= new QQuickView(QUrl(QLatin1String("qrc:/main.qml")));

       /// set data class datamanager to main qml
    mView->engine()->rootContext()->setContextProperty("GetData",this);
    mView->setColor(Qt::blue);
      QWidget *widghet  =createWindowContainer(mView,this);
    widghet->setMinimumSize(60,180);
    widghet->setMaximumSize(60,180);
    widghet->setStyleSheet("background-color: blue;");

    setStyleSheet("background-color: yellow;");
}

void CameraManipulatorWidget::setZoomStep(double zoomstep)
{
    mZoomStep=zoomstep;
}

void CameraManipulatorWidget::setUpDownStep(double updownstep)
{
    mUpDownstep=updownstep;
}

void CameraManipulatorWidget::setLeftRightStep(double leftrightstep)
{
    mLeftRightStep=leftrightstep;
}

void CameraManipulatorWidget::setPitchStep(double pitch)
{
    mPitchStep=pitch;

}

void CameraManipulatorWidget::setHeadStep(double head)
{
    mHeadStep =head;
}

void CameraManipulatorWidget::onZoomOutClicked()
{
    emit zoomChanged(-(mZoomStep));
}

void CameraManipulatorWidget::onUPClicked()
{
    emit upDownChanged(mUpDownstep);
}

void CameraManipulatorWidget::onDownClicked()
{
    emit upDownChanged(-(mUpDownstep));
}

void CameraManipulatorWidget::onRightClicked()
{
    emit leftRightChanged(mLeftRightStep);
}

void CameraManipulatorWidget::onLeftClicked()
{
    emit leftRightChanged(-(mLeftRightStep));
}

void CameraManipulatorWidget::onPitchUpClicked()
{
     emit pitchChanged(mPitchStep);
}

void CameraManipulatorWidget::onPitchDownClicked()
{
    emit pitchChanged(-(mPitchStep));
}

void CameraManipulatorWidget::onHeadUpClicked()
{
    emit headChanged(mHeadStep);
}

void CameraManipulatorWidget::onHeadDownClicked()
{
    emit headChanged(-(mHeadStep));
}


void CameraManipulatorWidget::onZoomInClicked()
{
    emit zoomChanged(mZoomStep);
}



void CameraManipulatorWidget::onHomeClicked()
{
    emit homeClicked();

}






