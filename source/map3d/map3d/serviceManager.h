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
    QString categoryIconSrc;
};

struct StatusNodeData: public osg::Referenced
{
    int id{-1};
    std::string name;
    double longitude;
    double latitude;
    double altitude;
    std::string command{"ADD"};
    ParenticAnnotationLayer* layer{nullptr};
    std::vector<NodeFieldData> fieldData;
};

struct NodeData: public osg::Referenced
{
    int id;
    std::string name;
    std::string type;
    std::string url2D;
    std::string url3D;
    std::string color;
    std::string imgSrc;
    std::string iconSrc;
    double latitude;
    double longitude;
    double altitude;
    double speed;
    std::string command{"ADD"};
    std::vector<ParenticAnnotationLayer*> layers;
    std::vector<NodeFieldData> fieldData;
};

struct PolyLineData: public osg::Referenced
{
    int id;
    std::string name;
    std::string color;
    int width;
    std::string command{"ADD"};
    ParenticAnnotationLayer* layer{nullptr};
    std::vector<osg::Vec3d> points;
};
struct PolygonData: public PolyLineData
{
    std::string fillColor;
};
struct CircleData: public osg::Referenced
{
    int id;
    std::string name;
    std::string color;
    double latitude;
    double longitude;
    double altitude;
    double radius;
    std::string command{"ADD"};
    ParenticAnnotationLayer* layer{nullptr};
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

    void layersData(QJsonObject layers);
    void flyableNodeData(QJsonObject jsonObject);
    void statusNodeData(QJsonObject jsonObject);
    void messageData(QString jsonData);
    void polylineData(QJsonObject polyline);
    void movableNodeData(QJsonObject jsonObject);

//    void addPolygon(QJsonDocument *polygon);
//    void addSphere(QJsonDocument *sphere);
//    void addCircle(QJsonDocument *circle);

signals:
    void layerDataReceived(CompositeAnnotationLayer *layer);
    void flyableNodeDataReceived(NodeData *modelNodeData);
    void statusNodeDataReceived(StatusNodeData *statusNodeData);
    void lineNodeDataReceived(PolyLineData *lineNodeData);
    void movableNodeDataReceived(NodeData *modelNodeData);
    void nodeDataReceived(NodeData *nodeData);
    void circleDataReceived(CircleData *circleData);
    void polygonDataReceived(PolygonData *polygonData);
private:
    void nodeData(QJsonObject jsonObject);
    void circleData(QJsonObject jsonObject);
    void polygonData(QJsonObject jsonObject);

    void parseLayersFromJson(QJsonObject jsonObject, CompositeAnnotationLayer *parent = nullptr);
    ParenticAnnotationLayer* findParenticLayer(int id);
private:
    MapItem *mMapItem{nullptr};
    QMap<int, ParenticAnnotationLayer*> mParenticLayerMap;
};

#endif // DATAMANAGER_H
