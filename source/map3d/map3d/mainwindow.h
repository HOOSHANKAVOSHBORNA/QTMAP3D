#ifndef MainWindow_H
#define MainWindow_H

#include <QQmlComponent>
#include "osgquickwindow.h"

class CrystalPluginInfo;
class PluginInterface;

class MainWindow : public OsgQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(qreal headingAngle READ headingAngle WRITE setHeadingAngle NOTIFY headingAngleChanged)

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

    qreal headingAngle() const;

signals:
    void sideItemCreated(int index, QObject *pSideItem);
    void toolboxItemCreated(const QString& itemName, const QString& categoryName,
                            QObject *menuItem,
                            PluginInterface *pInterface);
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

public slots:
    void initializePluginsUI(std::list<CrystalPluginInfo> pluginsInfoList);
    void setHeadingAngle(qreal angle);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    qreal mheadingAngle = 0.0;
};

#endif // MainWindow_H
