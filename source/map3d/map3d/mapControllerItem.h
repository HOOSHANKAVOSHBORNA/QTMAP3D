#ifndef MAPCONTROLLERITEM_H
#define MAPCONTROLLERITEM_H

#include <QObject>
#include <QTime>
#include "mapItem.h"

class MapControllerItem : public MapItem
{
    Q_OBJECT

    Q_PROPERTY(qreal headingAngle READ headingAngle NOTIFY headingAngleChanged)

    Q_PROPERTY(bool zoomInButtonPressed       WRITE setZoomInButtonPressed      )
    Q_PROPERTY(bool zoomOutButtonPressed      WRITE setZoomOutButtonPressed     )
    Q_PROPERTY(bool upButtonPressed           WRITE setUpButtonPressed          )
    Q_PROPERTY(bool downButtonPressed         WRITE setdownButtonPressed        )
    Q_PROPERTY(bool leftButtonPressed         WRITE setleftButtonPressed        )
    Q_PROPERTY(bool rightButtonPressed        WRITE setrightButtonPressed       )
    Q_PROPERTY(bool rotateUpButtonPressed     WRITE setrotateUpButtonPressed    )
    Q_PROPERTY(bool rotateDownButtonPressed   WRITE setrotateDownButtonPressed  )
    Q_PROPERTY(bool rotateLeftButtonPressed   WRITE setrotateLeftButtonPressed  )
    Q_PROPERTY(bool rotateRightButtonPressed  WRITE setrotateRightButtonPressed )

    Q_PROPERTY(QVector3D mapMouseGeoLocation READ mapMouseGeoLocation NOTIFY mouseLocationChanged)
    Q_PROPERTY(QVector3D mapMouseLocation READ mapMouseLocation NOTIFY mouseLocationChanged)

public:
    explicit MapControllerItem();

    QVector3D mapMouseGeoLocation() const;
    QVector3D mapMouseLocation() const;
    virtual void frame() override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
public slots:
    void setZoomInButtonPressed(bool pressed);
    void setZoomOutButtonPressed(bool pressed);
    void setUpButtonPressed(bool pressed);
    void setdownButtonPressed(bool pressed);
    void setleftButtonPressed(bool pressed);
    void setrightButtonPressed(bool pressed);
    void setrotateUpButtonPressed(bool pressed);
    void setrotateDownButtonPressed(bool pressed);
    void setrotateLeftButtonPressed(bool pressed);
    void setrotateRightButtonPressed(bool pressed);

signals:
    void headingAngleChanged();
    void mouseLocationChanged();
    void mousePointingLocationChanged(QVector3D location);
    void clicked();
private:
    void tickNavigation(double deltaTime);
private:
//    MapItem *mMapItem = nullptr;

private:
    bool mZoomInButtonPressed{false};
    bool mZoomOutButtonPressed{false};
    bool mUpButtonPressed{false};
    bool mDownButtonPressed{false};
    bool mLeftButtonPressed{false};
    bool mRightButtonPressed{false};
    bool mRotateUpButtonPressed{false};
    bool mRotateDownButtonPressed{false};
    bool mRotateLeftButtonPressed{false};
    bool mRotateRightButtonPressed{false};
    osgEarth::GeoPoint mCurrentMouseGeoPoint;

    QTime mLastMousePressTime = QTime::currentTime();
    QPoint mLastPressPoint = QPoint();
    bool mMousePressOusideClickProcess = false;
    bool mInClickProcess = false;
};

#endif // MAPCONTROLLERITEM_H
