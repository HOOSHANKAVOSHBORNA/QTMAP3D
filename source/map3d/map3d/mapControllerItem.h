#ifndef MAPCONTROLLERITEM_H
#define MAPCONTROLLERITEM_H

#include <QObject>
#include <QQmlEngine>
#include <QTime>
#include "mapItem.h"
#include "searchNodeModel.h"
Q_DECLARE_METATYPE(SearchNodeModel)
class SmallMap;
class MapControllerItem : public MapItem
{
    Q_OBJECT

    Q_PROPERTY(QVector2D compassDirection READ compassDirection WRITE setCompassDirection NOTIFY compassDirectionChanged FINAL)
    Q_PROPERTY(double fps READ fps WRITE setFps NOTIFY fpsChanged)

    Q_PROPERTY(bool zoomInButtonPressed WRITE setZoomInButtonPressed)
    Q_PROPERTY(bool zoomOutButtonPressed WRITE setZoomOutButtonPressed)

    Q_PROPERTY(QVector2D movePosition WRITE setMovePosition)
    Q_PROPERTY(QVector2D rotatePosition WRITE setRotatePosition)

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

    void setQmlEngine(QQmlEngine *newQmlEngine);

public slots:
    void setFps(double fps);
    void home();
    void setHeadingToNorth();
    void setZoomInButtonPressed(bool pressed);
    void setZoomOutButtonPressed(bool pressed);

    void setMovePosition(QVector2D &position);
    void setRotatePosition(QVector2D &position);


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

    QVector2D mMovePosition;
    QVector2D mRotatePosition;

    bool mMoveTick{false};
    bool mRotateTick{false};
    osgEarth::GeoPoint mCurrentMouseGeoPoint;

    QTime mLastMousePressTime = QTime::currentTime();
    QPoint mLastPressPoint = QPoint();
    bool mMousePressOusideClickProcess = false;
    bool mInClickProcess = false;
    double mFps = 0.0f;
    SearchNodeProxyModel* mSearchNodeProxyModel{nullptr};
    QVector2D m_compassDirection;
    MapItem *mSmallMap{nullptr};
    QQmlEngine *mQmlEngine{nullptr};
};

#endif // MAPCONTROLLERITEM_H
