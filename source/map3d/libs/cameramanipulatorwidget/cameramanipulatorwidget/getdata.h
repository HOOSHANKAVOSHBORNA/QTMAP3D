#ifndef GETDATA_H
#define GETDATA_H

#include <QObject>

class GetData : public QObject
{
    Q_OBJECT
public:
    explicit GetData(QObject *parent = nullptr);
    void setZoomStep(int zoomstep);
    void setUpDownStep (int updownstep);
    void setLeftRightStep(int leftrightstep);
    void setPitchStep(int pitch);
    void setHeadStep(int head);

public slots:
     Q_INVOKABLE void onZoomClicked(int zoom);
     Q_INVOKABLE void onUPDownClicked(int updown);
     Q_INVOKABLE void onLeftRightClicked(int left_right);
     Q_INVOKABLE void onPitchClicked(int pitch);
     Q_INVOKABLE void onHeadClicked(int head);
     Q_INVOKABLE void onHomeClicked();



    Q_INVOKABLE  int getZoomStep()  const;
    Q_INVOKABLE  int getUpDownStep() const ;
    Q_INVOKABLE  int getLeftRightStep() const;
    Q_INVOKABLE  int getPitchStep() const;
    Q_INVOKABLE  int getHeadStep() const;

signals:
     void zoomChanged(float newZoom);
     void upDownChanged(int newUpDown);
     void  leftRightChanged(int nemLeftRight);
     void pitchChanged(int newBearing);
     void  headChanged (int newTilt);
     void homeClicked ();

private:
     int    mZoomStep;
     int    mUpDownstep;
     int    mLeftRightStep;
     int    mHeadStep;
     int    mPitchStep;


};

#endif // GETDATA_H
