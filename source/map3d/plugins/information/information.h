#ifndef INFORMATION_H
#define INFORMATION_H


#include "statusNode.h"
#include "statusFilter.h"
#include "compositeAnnotationLayer.h"
#include "mapPin.h"
#include "plugininterface.h"
#include "qcolordialog.h"
#include <osgEarthAnnotation/PlaceNode>
#include <QObject>

#define CATEGORY  "Information"
#define MAPPIN    "MapPin"
#define STATUS    "Status"

class Information: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "information.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        NONE,
        READY,
        MOVING,
        CANCEL,
        CONFIRM
    };

    enum class Type{
        NONE,
        MapPin,
        Status
    };

    Information(QObject *parent = nullptr);
    bool setup() override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    void addUpdateStatusNode(const StatusNodeData &statusnNodeData);

public slots:
    void onMapPinItemCheck(bool check);
    void onStatusItemCheck(bool check);
    void statusNodeReceived(const StatusNodeData &statusNodeData);

protected:
    void initModel(const osgEarth::GeoPoint &geoPos);
    void moving(const osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancel();
private:
    void makeIconNode(const QString &fileName);
private:
    static int mCount;
    State mState{State::NONE};
    Type mType{Type::NONE};
    MapPin *mMapPin;
    QColorDialog *mColorDialog;
    osg::Vec4f mObjectColor;

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mInformationLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mMapPinLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mStatusLayer{nullptr};

    osg::ref_ptr<StatusNode> mStatusNode {nullptr};
    QMap<int, osg::ref_ptr<StatusNode>> mStatusNodeMap;
    StatusFilter *mStatusFilter{nullptr};
};


#endif // INFORMATION_H
