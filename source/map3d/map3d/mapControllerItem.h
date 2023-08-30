#ifndef MAPCONTROLLERITEM_H
#define MAPCONTROLLERITEM_H

#include <QObject>
#include <QTime>
#include "mapItem.h"
#include "searchNodeModel.h"
Q_DECLARE_METATYPE(SearchNodeModel)

class MapControllerItem : public MapItem
{
    Q_OBJECT

    Q_PROPERTY(QVector2D compassDirection READ compassDirection WRITE setCompassDirection NOTIFY compassDirectionChanged FINAL)
    Q_PROPERTY(double fps READ fps WRITE setFps NOTIFY fpsChanged)

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

//    Q_PROPERTY(SearchNodeModel searchNodeModel   READ getSearchNodeModel NOTIFY searchChange)
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
    double fps() const;

    QVector2D compassDirection() const;
    void setCompassDirection(const QVector2D &newCompassDirection);

public slots:
    void setFps(double fps);
    void home();
    void setHeadingToNorth();
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
    SearchNodeProxyModel *searchNodeProxyModel() const;

signals:
    void fpsChanged();
    void mouseLocationChanged();
    void mousePointingLocationChanged(QVector3D location);
    void clicked();
    void searchChange();
    void compassDirectionChanged();

private:
    void tickNavigation(double deltaTime);
    void calculateNavigationStep();
    void calculateFps();
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
    double mFps = 0.0f;
    SearchNodeProxyModel* mSearchNodeProxyModel{nullptr};
    QVector2D m_compassDirection;
};

#endif // MAPCONTROLLERITEM_H
