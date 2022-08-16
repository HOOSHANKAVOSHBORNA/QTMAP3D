#ifndef CAMERAMANIPULATORWIDGET_H
#define CAMERAMANIPULATORWIDGET_H

#include "cameramanipulatorwidget_global.h"
#include <QWidget>
#include <QQuickView>
#include <QQuickWidget>
//  library for display qml  in widget
#include <QQmlContext>
#include <QQmlEngine>

class CAMERAMANIPULATORWIDGET_EXPORT CameraManipulatorWidget :public QWidget
{
    Q_OBJECT
public:
    CameraManipulatorWidget(QWidget *parent = nullptr);

    void setZoomStep(double zoomstep);
    void setUpDownStep (double updownstep);
    void setLeftRightStep(double leftrightstep);
    void setPitchStep(double pitch);
    void setHeadStep(double head);


public slots:
     Q_INVOKABLE void onZoomInClicked();
     Q_INVOKABLE void onZoomOutClicked();
     Q_INVOKABLE void onUPClicked();
     Q_INVOKABLE void onDownClicked();
     Q_INVOKABLE void onRightClicked();
     Q_INVOKABLE void onLeftClicked();
     Q_INVOKABLE void onPitchUpClicked();
     Q_INVOKABLE void onPitchDownClicked();
     Q_INVOKABLE void onHeadUpClicked();
    Q_INVOKABLE void onHeadDownClicked();
     Q_INVOKABLE void onHomeClicked();




signals:
     void zoomChanged(double newZoom);
     void upDownChanged(double newUpDown);
     void  leftRightChanged(double nemLeftRight);
     void pitchChanged(double newBearing);
     void  headChanged (double newTilt);
     void homeClicked ();
private:
    QQuickWidget *mQQuickWidget;
    double    mZoomStep;
    double    mUpDownstep;
    double    mLeftRightStep;
    double    mHeadStep;
    double    mPitchStep;

};

#endif // CAMERAMANIPULATORWIDGET_H
