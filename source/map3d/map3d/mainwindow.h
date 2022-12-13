#ifndef MainWindow_H
#define MainWindow_H

#include <QQmlComponent>
#include <QVector3D>
#include "osgquickwindow.h"

#include "pluginmanager.h"

class PluginInfo;
class PluginInterface;
class InfoWidgetHandle;


class MainWindow : public OsgQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(qreal headingAngle READ headingAngle WRITE setHeadingAngle NOTIFY headingAngleChanged)
    Q_PROPERTY(QVector3D mousePointingLocationWgs84 READ mousePointingLocationWgs84 WRITE setMousePointingLocationWgs84 NOTIFY mousePointingLocationWgs84Changed)
    Q_PROPERTY(QVector3D mousePointingLocation READ mousePointingLocation WRITE setMousePointingLocation NOTIFY mousePointingLocationChanged)


    Q_PROPERTY(qreal focalPointLat   READ focalPointLat   WRITE setFocalPointLat   NOTIFY focalPointLatChanged  )
    Q_PROPERTY(qreal focalPointLong  READ focalPointLong  WRITE setFocalPointLong  NOTIFY focalPointLongChanged )
    Q_PROPERTY(qreal focalPointRange READ focalPointRange WRITE setFocalPointRange NOTIFY focalPointRangeChanged)
    Q_PROPERTY(qreal focalPointPitch READ focalPointPitch WRITE setFocalPointPitch NOTIFY focalPointPitchChanged)
    Q_PROPERTY(qreal focalPointHead READ focalPointHead WRITE setFocalPointHead NOTIFY focalPointHeadChanged)
    Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)

    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

    qreal headingAngle() const;
    QVector3D mousePointingLocationWgs84() const;
    QVector3D mousePointingLocation() const;

    qreal focalPointLat  () const;
    qreal focalPointLong () const;
    qreal focalPointRange() const;
    qreal focalPointPitch() const;
    qreal focalPointHead() const;


    qreal fps() const;

    InfoWidgetHandle *infoWidgetHandle() const;

signals:
    void sideItemCreated(int index, QObject *pSideItem);
    void toolboxItemCreated(ItemDescProxy *itemProxy);
    void fileItemCreated(ItemDescProxy *itemProxy);
    void toolboxItemClicked(const QString& itemName, const QString& categoryName);
    void fileItemClicked(const QString& itemName, const QString& categoryName);
    void toolboxItemCheckedChanged(const QString& itemName,
                                   const QString& categoryName,
                                   bool checked);


    void infoWidget2D3DButtonClicked();
    void infoWidgetRouteButtonClicked();
    void infoWidgetFollowButtonClicked();
    void infoWidgetMoreButtonClicked();

    void homeButtonClicked();
    void projectionButtonClicked();
    void zoomInButtonClicked();
    void zoomOutButtonClicked();
    void upButtonClicked();
    void downButtonClicked();
    void leftButtonClicked();
    void rightButtonClicked();
    void rotateUpButtonClicked();
    void rotateDownButtonClicked();
    void rotateLeftButtonClicked();
    void rotateRightButtonClicked();

    void headingAngleChanged(qreal angle);
    void mousePointingLocationWgs84Changed();
    void mousePointingLocationChanged();
    void goToLocation(qreal lat, qreal lon, qreal range);

    void focalPointLatChanged  ();
    void focalPointLongChanged ();
    void focalPointRangeChanged();
    void focalPointPitchChanged();
    void focalPointHeadChanged();

    void fpsChanged();

public slots:
    void initializePluginsUI(std::list<PluginInfo>& pluginsInfoList);
    void setHeadingAngle(qreal angle);
    void setMousePointingLocationWgs84(const QVector3D& pointedLoc);
    void setMousePointingLocation(const QVector3D& pointedLoc);

    void setFocalPointLat  (qreal focalPointLat) ;
    void setFocalPointLong (qreal focalPointLong);
    void setFocalPointRange(qreal focalPointRange) ;
    void setFocalPointPitch(qreal focalPointPitch) ;
    void setFocalPointHead(qreal focalPointHead) ;

    void setFps(qreal fps);

    void travelToViewpoint(qreal latitude,
                           qreal longitude,
                           qreal range,
                           qreal pitch,
                           qreal heading);

    void showInfoWidget(InfoWidgetType infoWidgetType);
    void updateInfoWidgetData(const QString& dataJSON);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    qreal mheadingAngle = 0.0;
    QVector3D mMousePointingLocationWgs84;
    QVector3D mMousePointingLocation;

    qreal mFocalPointLat   = 0.0;
    qreal mFocalPointLong  = 0.0;
    qreal mFocalPointRange = 0.0;
    qreal mFocalPointPitch = 0.0;
    qreal mFocalPointHead = 0.0;

    qreal mFps = 0.0f;

    InfoWidgetHandle *mInfoWidgetHandle = nullptr;
};

#endif // MainWindow_H
