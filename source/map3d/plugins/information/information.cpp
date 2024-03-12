#include "information.h"
#include <osgEarth/GLUtils>


using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
int Information::mCount{0};

Information::Information(QObject *parent): PluginInterface(parent)
{
}

bool Information::setup()
{
    mStatusFilter = new StatusFilter(qmlEngine(),mainWindow());
   connect(serviceManager(), &ServiceManager::statusNodeDataReceived, this, &Information::statusNodeReceived);

    mInformationLayer = new CompositeAnnotationLayer;
    mInformationLayer->setName(CATEGORY);
    mapItem()->getMapObject()->addLayer(mInformationLayer);

    ///////////////////////////add map pin/////////////////////////////////
    auto mapPinToolBoxItem =  new ToolboxItem{MAPPIN, CATEGORY, "qrc:/resources/mappin.png", true};
    QObject::connect(mapPinToolBoxItem, &ToolboxItem::itemChecked, this, &Information::onMapPinItemCheck);
    toolbox()->addItem(mapPinToolBoxItem);


    auto statusToolboxItem =  new ToolboxItem{STATUS, CATEGORY, "qrc:/resources/status.png", true};
    QObject::connect(statusToolboxItem, &ToolboxItem::itemChecked, this, &Information::onStatusItemCheck);
    toolbox()->addItem(statusToolboxItem);

    mStatusLayer = new ParenticAnnotationLayer();
    mStatusLayer->setName(STATUS);
    return true;
}

bool Information::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::READY) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            initModel(geoPos);
            return true;
        }
        if (mState == State::MOVING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            moving(geoPos);
            return true;
        }
    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancel();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        confirm();
        return false;
    }
    return false;
}

void Information::onMapPinItemCheck(bool check)
{
    if (check) {
        QColorDialog* colorDialog = new QColorDialog();
        colorDialog->exec();
        if (colorDialog->result() == QDialog::Accepted) {
            mObjectColor.r() = colorDialog->selectedColor().redF();
            mObjectColor.g() = colorDialog->selectedColor().greenF();
            mObjectColor.b() = colorDialog->selectedColor().blueF();
            mObjectColor.a() = colorDialog->selectedColor().alphaF();
        }
        mState = State::READY;
        mType = Type::MapPin;
        delete colorDialog;
    }else {
        if(mState == State::MOVING)
            cancel();
        mState = State::NONE;
        mType = Type::NONE;
    }
}

void Information::onStatusItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/status.png");
        mType = Type::Status;
        mState = State::READY;
        mapItem()->addNode(mIconNode);
    }
    else {
        if(mState == State::MOVING)
            cancel();

        mState = State::NONE;
        mapItem()->removeNode(mIconNode);
    }
}

void Information::initModel(const osgEarth::GeoPoint &geoPos)
{
    QString name;

    switch (mType) {
    case Type::MapPin:
        mMapPinLayer = new ParenticAnnotationLayer;
        mMapPinLayer->setName(MAPPIN);
        mMapPin = new MapPin(mapItem(),mObjectColor);
        mMapPin->setPosition(geoPos);
        mMapPinLayer->addChild(mMapPin);
        mInformationLayer->addLayer(mMapPinLayer);
        break;
    case Type::Status:
        name = "Status" + QString::number(mCount);
        mStatusNode = new StatusNode(mapItem());
        {
            NodeFieldData data;
            data.name = "name";
            data.value = 10;
            NodeFieldData data1;
            data1.name = "name";
            data1.value = 30000;
            NodeFieldData data2;
            data2.name = "name";
            data2.value = "kasjdf";
            std::vector<NodeFieldData> dataList;
            dataList.push_back(data);
            dataList.push_back(data1);
            dataList.push_back(data2);
            mStatusNode->setFieldData("title", dataList);
        }
        mStatusNode->setPosition(geoPos);

        if(!mInformationLayer->containsLayer(mStatusLayer)){
            mStatusLayer->clear();
            mInformationLayer->addLayer(mStatusLayer);
        }
        mStatusLayer->addChild(mStatusNode);
        mStatusLayer->setName(name.toStdString());
        break;
    default:
        break;
    }
    mState = State::MOVING;
}

void Information::moving(const osgEarth::GeoPoint &geoPos)
{
    switch (mType) {
    case Type::MapPin:
        mMapPin->setPosition(geoPos);
        break;
    case Type::Status:
            mStatusNode->setPosition(geoPos);
        break;
    default:
        break;
    }
}

void Information::confirm()
{
    if (mState == State::MOVING) {
        mState = State::READY;
    }
}

void Information::cancel(){

    if(mState == State::MOVING){
        switch (mType) {
        case Type::MapPin:
            if(!mMapPinLayer->children().isEmpty()){
                mMapPinLayer->removeChild(mMapPin);
                if(mMapPinLayer->children().isEmpty()){
                    mInformationLayer->removeLayer(mMapPinLayer);
                }
            }
            break;
        case Type::Status:
            mStatusLayer->removeChild(mStatusNode);
            break;

        default:
            break;
        }
        mState = State::READY;
        mStatusNode.release();
    }
}

void Information::addUpdateStatusNode(const StatusNodeData &statusnNodeData)
{
    auto layer = mapItem()->getMapObject()->getLayerByUserId(statusnNodeData.layerId);
    if (!layer)
        return;
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), statusnNodeData.longitude, statusnNodeData.latitude, statusnNodeData.altitude);
    osg::ref_ptr<StatusNode> statusNode;

    if(!mStatusNodeMap.contains(statusnNodeData.id)){
        statusNode = new StatusNode(mapItem());
        mStatusNodeMap[statusnNodeData.id] = statusNode;
    }
    else{
        statusNode = mStatusNodeMap[statusnNodeData.id];
        layer->removeChild(statusNode);
    }
    statusNode->setPosition(geoPoint);

    statusNode->setName(statusnNodeData.name.toStdString());
    statusNode->setNodeData(statusnNodeData);
    layer->addChild(statusNode);
}

void Information::statusNodeReceived(const StatusNodeData &statusNodeData)
{
    StatusNodeData filteredData = mStatusFilter->filterStatusData(statusNodeData);
    if (filteredData.command == Command::Remove){
        if (mStatusNodeMap.contains(filteredData.id)){
            auto layer = mapItem()->getMapObject()->getLayerByUserId(filteredData.layerId);
            if (layer)
                layer->removeChild(mStatusNodeMap[filteredData.id]);
            mStatusNodeMap[filteredData.id].release();
            mStatusNodeMap.remove(filteredData.id);
        }
    } else if (filteredData.command == Command::Update){
        addUpdateStatusNode(filteredData);
    } else {
        addUpdateStatusNode(filteredData);
    }
}

void Information::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}
