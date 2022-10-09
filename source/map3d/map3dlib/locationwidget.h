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
    void addLocation(QString name, double latitude, double longitude, double range);
    void setClose();

public slots:
    void setMousePosition(QString location);
    void setCurrentLocation(double latitude ,double longitude );

signals:
    void goPosition(float latitude ,float longitude ,float range);
    void saveLocation(QString name);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QQuickWidget *mQQuickWidget;
    QList <osgEarth::Viewpoint*> mListViewpoint;
private slots:

signals :
   void savePosition(QString str , double x  , double y , double range);
   void openWidget(bool a , bool b, bool c);
   void changePosition(QString location);
   void onCurrentClicked(QString name);
   void close();
   void currentLocation(double latitude ,double longitude);

};

#endif // LOCATIONWIDGET_H
