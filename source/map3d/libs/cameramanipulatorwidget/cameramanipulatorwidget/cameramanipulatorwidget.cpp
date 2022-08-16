#include "cameramanipulatorwidget.h"
CameraManipulatorWidget::CameraManipulatorWidget(QWidget *parent):
    QWidget(parent)
{
    mZoomStep=0.5;
    mUpDownstep=0.5;
    mLeftRightStep=0.5;
    mPitchStep=0.5;
    mHeadStep=0.5;
    mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/main.qml")),this);
            mQQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
            mQQuickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
            mQQuickWidget->setClearColor(Qt::transparent);
            mQQuickWidget->resize(60, 190);
            mQQuickWidget->raise();

       /// set data class datamanager to main qml
    mQQuickWidget->engine()-> rootContext()->setContextProperty("GetData",this);
    mQQuickWidget->setMinimumSize(60,190);
    mQQuickWidget->setMaximumSize(60,190);

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






