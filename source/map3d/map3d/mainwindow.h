#ifndef MainWindow_H
#define MainWindow_H

#include <QQmlComponent>
#include <QVector3D>
#include "osgquickwindow.h"

#include "pluginmanager.h"

class PluginInfo;
class PluginInterface;

class MainWindow : public OsgQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(qreal headingAngle READ headingAngle WRITE setHeadingAngle NOTIFY headingAngleChanged)
    Q_PROPERTY(QVector3D mousePointedLocation READ mousePointedLocation WRITE setMousePointedLocation NOTIFY mousePointedLocationChanged)

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

    qreal headingAngle() const;
    QVector3D mousePointedLocation() const;

signals:
    void sideItemCreated(int index, QObject *pSideItem);
    void toolboxItemCreated(ToolboxItemDescProxy *itemProxy);
    void toolboxItemClicked(const QString& itemName, const QString& categoryName);

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
    void mousePointedLocationChanged();
    void goToLocation(qreal lat, qreal long, qreal range);

public slots:
    void initializePluginsUI(std::list<PluginInfo> pluginsInfoList);
    void setHeadingAngle(qreal angle);
    void setMousePointedLocation(const QVector3D& pointedLoc);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    qreal mheadingAngle = 0.0;
    QVector3D mMousePointedLocation;
};

#endif // MainWindow_H
