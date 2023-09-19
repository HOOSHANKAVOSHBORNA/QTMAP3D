#ifndef LOCATION_H
#define LOCATION_H


#include "compositeAnnotationLayer.h"
#include "mapPin.h"
#include "plugininterface.h"
#include "qcolordialog.h"
#include <osgEarthAnnotation/PlaceNode>
#include <QObject>

#define CATEGORY  "Location"
#define MAPPIN    "MapPin"



class Location: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "location.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        NONE,
        READY,
        MOVING,
        CANCEL,
        CONFIRM
    };

    enum class Mode{
        NONE,
        MapPin
    };

    Location(QObject *parent = nullptr);
    bool setup() override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;




public slots:
    void onMapPinClicked(bool check);


protected:
    void add(const osgEarth::GeoPoint &geoPos);
    void moving(const osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancelAdd();


private:
//    MapItem *mapItem() const;
    State mState{State::NONE};
    Mode mMode{Mode::NONE};
    MapPin *mMapPin;
    QColorDialog *mColorDialog;
    osg::Vec4f mObjectColor;


    osg::ref_ptr<CompositeAnnotationLayer> mLocationLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mPinMapLayer{nullptr};


};


#endif // LOCATION_H
