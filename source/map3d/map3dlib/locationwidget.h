#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H

#include <QtQuickWidgets/QQuickWidget>
//#include <osgEarth/Viewpoint>

class QQuickWidget;
namespace  osgEarth{
class Viewpoint;
}


class LocationWidget : public QQuickWidget
{
    Q_OBJECT
public:
    explicit LocationWidget(QWidget *parent = nullptr);
    void addViewPoint(osgEarth::Viewpoint point);
    void setClose();

public slots:
    void setMousePosition(QString location);

signals:
    void goPosition(float latitude ,float longitude ,float range);
    void onClickedPosition(osgEarth::Viewpoint* point);
    void sendNamePosition(QString name);
    void currentLocation(double latitude ,double longitude );
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QQuickWidget *mQQuickWidget;
    QList <osgEarth::Viewpoint*> mListViewpoint;
private slots:

signals :
   void savePosition(QString str , double x  , double y);
   void openWidget(bool a , bool b, bool c);
   void changePosition(QString location);
   void onCurrentClicked(QString name);
   void close();
};

#endif // LOCATIONWIDGET_H
