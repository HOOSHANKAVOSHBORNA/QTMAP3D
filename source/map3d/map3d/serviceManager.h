#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "compositeAnnotationLayer.h"
#include "mapItem.h"
#include <QJsonDocument>
#include <osgEarth/Layer>

struct NodeFieldData
{
    QString name;
    QVariant value;
    QString category;
};

struct StatusNodeData: public osg::Referenced
{
    int id{-1};
    std::string name;
    double longitude;
    double latitude;
    double altitude;
    ParenticAnnotationLayer* layer{nullptr};
    std::vector<NodeFieldData> data;
};

struct NodeData: public osg::Referenced
{
    int id;
    std::string name;
    std::string url2D;
    std::string url3D;
    std::string color;
    double latitude;
    double longitude;
    double altitude;
    double speed;
    std::vector<ParenticAnnotationLayer*> layers;
    std::vector<NodeFieldData> data;
};


//struct ServiceLayer {
//    int id;
//    int parentId;
//    std::string text;
//    int order;
//    bool parentic;
//    ServiceLayer(int id, int parent, std::string text, int order, bool parentic):id{id}, parentId{parent}, text{text}, order{order}, parentic{parentic} {}

//};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(MapItem *mapItem, QObject *parent = nullptr);

    void layersData(QJsonObject jsonObject);
    void flyableNodeData(QJsonObject jsonObject);
    void statusNodeData(QJsonObject jsonObject);
    void messageData(QString jsonData);

//    void addPolygon(QJsonDocument *polygon);
//    void addSphere(QJsonDocument *sphere);
//    void addCircle(QJsonDocument *circle);
//    void addPolyline(QJsonDocument *polyline);

signals:
    void layerDataReceived(CompositeAnnotationLayer *layer);
    void flyableNodeDataReceived(NodeData *modelNodeData);
    void statusNodeDataReceived(StatusNodeData *statusNodeData);
private:
    void parseLayersFromJson(QJsonObject jsonObject, CompositeAnnotationLayer *parent = nullptr);

private:
    MapItem *mMapItem{nullptr};
    QMap<int, ParenticAnnotationLayer*> mParenticLayerMap;
};

#endif // DATAMANAGER_H
