#ifndef CAMERAMANIPULATORWIDGET_H
#define CAMERAMANIPULATORWIDGET_H

#include "cameramanipulatorwidget_global.h"
#include <QWidget>
#include <QQuickView>
//  library for display qml  in widget
#include <QQmlContext>
#include <QQmlEngine>
#include "getdata.h"

class CAMERAMANIPULATORWIDGET_EXPORT CameraManipulatorWidget :public QWidget
{
    Q_OBJECT
public:
    CameraManipulatorWidget(QWidget *parent = nullptr);

    void setZoomStep(int zoomstep);
    void setUpDownStep (int updownstep);
    void setLeftRightStep(int leftrightstep);
    void setPitchStep(int pitch);
    void setHeadStep(int head);


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
     void zoomChanged(float newZoom);
     void upDownChanged(int newUpDown);
     void  leftRightChanged(int nemLeftRight);
     void pitchChanged(int newBearing);
     void  headChanged (int newTilt);
     void homeClicked ();
private:
    GetData *mData;
    QQuickView *mView;
    int    mZoomStep=12;
    int    mUpDownstep;
    int    mLeftRightStep;
    int    mHeadStep;
    int    mPitchStep;

};

#endif // CAMERAMANIPULATORWIDGET_H
