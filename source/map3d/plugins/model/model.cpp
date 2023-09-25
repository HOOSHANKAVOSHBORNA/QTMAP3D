#include "model.h"
#include "mapItem.h"
#include "MoveableModelNode.h"
#include "flyableModelNode.h"
#include "qjsonobject.h"
#include "serviceManager.h"
#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarth/ModelSource>
#include <osgEarth/ModelSource>

#include <QPainter>
#include <QRandomGenerator>
#include <osgFX/Outline>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/StyleSheet>
#include <osg/ShapeDrawable>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
int Model::mCount{0};
Model::Model(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(model);
}

Model::~Model()
{
    mIconNode.release();
}

bool Model::setup()
{

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &Model::onModeChanged);
    mIs3D = mapItem()->getMode();

    //    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(serviceManager(), &ServiceManager::flyableAdded, this, &Model::addFlyableModel);

    mModelNodeLayer = new CompositeAnnotationLayer();
    mModelNodeLayer->setName(MODEL);
    mapItem()->getMapObject()->addLayer(mModelNodeLayer);


    auto treeToolboxItem =  new ToolboxItem{TREE, MODEL, "qrc:/resources/tree.png", true};
    QObject::connect(treeToolboxItem, &ToolboxItem::itemChecked, this, &Model::onTreeItemCheck);
    toolbox()->addItem(treeToolboxItem);

    auto carToolboxItem =  new ToolboxItem{CAR, MODEL, "qrc:/resources/car.png", true};
    QObject::connect(carToolboxItem, &ToolboxItem::itemChecked, this, &Model::onCarItemCheck);
    toolbox()->addItem(carToolboxItem);

    auto airplaneToolboxItem =  new ToolboxItem{AIRPLANE, MODEL, "qrc:/resources/airplane.png", true};
    QObject::connect(airplaneToolboxItem, &ToolboxItem::itemChecked, this, &Model::onAirplanItemCheck);
    toolbox()->addItem(airplaneToolboxItem);

    auto statusToolboxItem =  new ToolboxItem{STATUS, MODEL, "qrc:/resources/status.png", true};
    QObject::connect(statusToolboxItem, &ToolboxItem::itemChecked, this, &Model::onStatusItemCheck);
    toolbox()->addItem(statusToolboxItem);


    mSimpleNodeLayer = new ParenticAnnotationLayer();
    mSimpleNodeLayer->setName(TREE);

    mMoveableNodeLayer = new ParenticAnnotationLayer();
    mMoveableNodeLayer->setName(CAR);

    mFlyableNodelLayer = new ParenticAnnotationLayer();
    mFlyableNodelLayer->setName(AIRPLANE);

    mStatusNodelLayer = new ParenticAnnotationLayer();
    mStatusNodelLayer->setName(STATUS);
    return true;
}

void Model::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *Model::iconNode() const
{
    return mIconNode.get();
}

Model::State Model::state() const
{
    return mState;
}

void Model::setState(State newState)
{
    mState = newState;
}

bool Model::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
        if (mSelectedModelNode && mSelectedModelNode != modelNode){
//            mSelectedModelNode->getGeoTransform()->removeChild(mCircle);
//            mCircle = nullptr;
//            mSelectedModelNode->getGeoTransform()->removeChild(mCone);
//            mCone = nullptr;
            mSelectedModelNode->selectModel(false);
           // mSelectedModelNode->getOrCreateStateSet()->removeAttribute(mPm.get());
        }
        if(modelNode){

            mSelectedModelNode = modelNode;
            mClicked = true;
//            qDebug()<<"model name: "<<mSelectedModelNode->getName();
            mSelectedModelNode->selectModel(true);

            mPm = new osg::PolygonMode;
            mPm->setMode(osg::PolygonMode::FRONT_AND_BACK,
                        osg::PolygonMode::LINE);

            //mSelectedModelNode->getOrCreateStateSet()->setAttribute(mPm.get());







//            osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline;
//            outline->setWidth( 2 );
//            outline->setColor( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
//            outline->addChild(mSelectedModelNode);
//            mapItem()->getViewer()->getCamera()->setClearMask(
//                GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT
//                );
//            mapItem()->getViewer()->setSceneData( outline.get() );
        }
    if(mState == State::NONE)
        return false;
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
        //mCurrentModel->setScalability(false);

        StatusNode::Data data0;
        data0.name = "x";
        data0.value = 85;
        StatusNode::Data data1;
        data1.name = "y";
        data1.value = 69;
        StatusNode::Data data2;
        data2.name = "z";
        data2.value = 6985;
        StatusNode::Data data3;
        data3.name = "u";
        data3.value = "BooB";


        std::list<StatusNode::Data> dataList;
        dataList.push_back(data0);
        dataList.push_back(data1);
        dataList.push_back(data2);
        dataList.push_back(data3);

        mStatusModel->setData("kos",dataList);
        //confirm();
        return false;
    }

    mClicked = false;
    return false;
}

bool Model::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }


    //    if(mState == State::MOVING){
    //        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
    //        moving(geoPos);
    //        return true;
    //    }
    //--------------------------------
    return false;
}

bool Model::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{


//    if (mCircle){
//        mCircle->setScale(mSelectedModelNode->getScale());
//        mCone->setScale(mSelectedModelNode->getScale());

//    }


    return false;
}

osgEarth::Symbology::Style &Model::getDefaultStyle()
{
    static osgEarth::Symbology::Style _style;
    static bool bFirst = true;
    if (bFirst) {
        static osg::Node *node = new osg::Node;
        _style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
        bFirst = false;
    }

    return _style;
}

void Model::onTreeItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/tree.png");

        mType = Type::SIMPLE;

        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}


void Model::onCarItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/car.png");
        mType = Type::MOVEABLE;

        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void Model::onAirplanItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/airplane.png");

        mType = Type::FLYABLE;
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void Model::onStatusItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/status.png");

        mType = Type::INFO;
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}


void Model::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D && mCircle){

//        mCircle->setRadius(osgEarth::Distance(mCurrentModelSize, osgEarth::Units::METERS));
//        mCone->setRadius(osgEarth::Distance(mCurrentModelSize/2, osgEarth::Units::METERS));
//        mCone->setHeight(osgEarth::Distance(mCurrentModelSize/2, osgEarth::Units::METERS));
    }
    else if(!mIs3D && mSphere){
//        mCircle->setRadius(osgEarth::Distance(3, osgEarth::Units::METERS));
    }
}


void Model::addFlyableModel(ServiceFlyableModel *serviceModel)
{
    FlyableModelNode *fmodel = new FlyableModelNode(mapItem(), serviceModel->url3D, serviceModel->url2D);
    fmodel->setName(serviceModel->name);
    double latitude{serviceModel->latitude};
    double longitude{serviceModel->longitude};
    double altitude{serviceModel->altitude};
    osgEarth::GeoPoint geopos(mapItem()->getMapSRS(), longitude, latitude, altitude);
    fmodel->setPosition(geopos);
//    fmodel.setHeading
    fmodel->setSpeed(serviceModel->speed);
//    ParenticAnnotationLayer *p = mapItem()->getMapObject()->getLayerByUserId(serviceModel->id);
    ParenticAnnotationLayer *p = new ParenticAnnotationLayer;
    p->setName("seee");
    mapItem()->getMapObject()->addLayer(p);
    p->addChild(fmodel);
//    if (p)
//        p->addChild(fmodel);
}

void Model::initModel(const osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::SIMPLE:
        name = "Tree" + QString::number(mCount);
        mCurrentModel = new SimpleModelNode(mapItem(),"../data/models/tree_I.osgb", "../data/images/model/tree.png");
        if(!mModelNodeLayer->containsLayer(mSimpleNodeLayer)){
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
        mSimpleNodeLayer->addChild(mCurrentModel);
        break;
    case Type::MOVEABLE:
        name = "Car" + QString::number(mCount);
        mCurrentModel = new MoveableModelNode(mapItem(),"../data/models/car.osgb", "../data/images/model/car.png");
        if(!mModelNodeLayer->containsLayer(mMoveableNodeLayer)){
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
        mMoveableNodeLayer->addChild(mCurrentModel);
        break;
    case Type::FLYABLE:
        name = "Airplane" + QString::number(mCount);
        mCurrentModel = new FlyableModelNode(mapItem(),"../data/models/aircraft/boeing-747.osgb", "../data/images/model/airplane.png");
        if(!mModelNodeLayer->containsLayer(mFlyableNodelLayer)){
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
        mFlyableNodelLayer->addChild(mCurrentModel);
        break;
    case Type::INFO:
        name = "Status" + QString::number(mCount);
        mStatusModel = new StatusNode(mapItem());

        if(!mModelNodeLayer->containsLayer(mStatusNodelLayer)){
            mStatusNodelLayer->clear();
            mModelNodeLayer->addLayer(mStatusNodelLayer);
        }
        mStatusNodelLayer->addChild(mStatusModel);
        break;
    default:
        break;
    }

    if (mCurrentModel){
        mCurrentModel->setName(name.toStdString());
        mCurrentModel->setPosition(geoPos);
    }
    else{
        mStatusModel->setName(name.toStdString());
        mStatusModel->setPosition(geoPos);
    }


;
    setState(State::MOVING);
    mCount++;
}

void Model::moving(osgEarth::GeoPoint &geoPos){

    if (mCurrentModel){
        if (mCurrentModel->asFlyableModelNode()){
            double randomHeight = 50 + (QRandomGenerator::global()->generate() % (100 - 50));
            geoPos.z() += randomHeight;
            mCurrentModel->asFlyableModelNode()->flyTo(geoPos,20);
            return;
        }
        if (mCurrentModel->asMoveableModelNode()){
            mCurrentModel->asMoveableModelNode()->moveTo(geoPos,20);
            return;
        }
    }
    if (mCurrentModel){
        mCurrentModel->setPosition(geoPos);
    }
    else{
        mStatusModel->setPosition(geoPos);
    }
}

void Model::confirm()
{
    if (state() == State::MOVING) {
        setState(State::READY);
    }
}

void Model::cancel(){

    if(state() == State::MOVING){
        switch (mType) {
        case Type::SIMPLE:
            mSimpleNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::MOVEABLE:
            mMoveableNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::FLYABLE:
            mFlyableNodelLayer->removeChild(mCurrentModel);
            break;
        case Type::INFO:
            mStatusNodelLayer->removeChild(mStatusModel);
            break;
        default:
            break;
        }
        mCurrentModel.release();
        mStatusModel.release();
        setState(State::READY);
        mCount--;
    }
}

SimpleModelNode *Model::pick(float x, float y)
{
    SimpleModelNode *simpleModelNode = nullptr;
    osgViewer::Viewer *viewer = mapItem()->getViewer();
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(x, y, intersections))
    {
        for(const osgUtil::LineSegmentIntersector::Intersection& hit : intersections)
        {
            const osg::NodePath& nodePath = hit.nodePath;
            for(osg::NodePath::const_iterator nitr=nodePath.begin();
                 nitr!=nodePath.end();
                 ++nitr)
            {
                simpleModelNode = dynamic_cast<SimpleModelNode*>(*nitr);
                if (simpleModelNode)
                {
                    osg::ref_ptr<osg::MatrixTransform> _selectionBox;
                    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                    geode->addDrawable(
                        new osg::ShapeDrawable(new osg::Box(osg::Vec3(),
                                                            1.0f)) );
                    _selectionBox = new osg::MatrixTransform;
                    _selectionBox->setNodeMask( 0x1 );
                    _selectionBox->addChild( geode.get() );
                    osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
                    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
                    ss->setAttributeAndModes(new osg::PolygonMode(
                        osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));

                    osg::BoundingBox bb = hit.drawable->getBoundingBox();
                    qDebug()<<"radius: "<<bb.radius();
                    osg::Vec3 worldCenter = bb.center() *
                                            osg::computeLocalToWorld(hit.nodePath);
                    _selectionBox->setMatrix(
                        osg::Matrix::scale(bb.xMax()-bb.xMin(),
                                           bb.yMax()-bb.yMin(),
                                           bb.zMax()-bb.zMin()) *
                        osg::Matrix::translate(worldCenter) );



//                    if (!mCircle){
//                        //---circle indicator------------------------------------------------------
//                        mCircle = new Circle();
//                        mCurrentModelSize = hit.drawable->getBoundingBox().yMax();
//                        mCircle->setFillColor(osg::Vec4f(0,0.6,0.6,0));
//                        mCircle->setStrokeColor(osg::Vec4f(0,1,0,1));
//                        mCircle->setStrokeWidth(2);
//                        mCircle->setRadius(osgEarth::Distance(mCurrentModelSize, osgEarth::Units::METERS));
//                        simpleModelNode->getPositionAttitudeTransform()->setPivotPoint(osg::Vec3d(-0.2, -0.15, 0));
//                        simpleModelNode->getGeoTransform()->addChild(mCircle);
//                        mCircle->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.01));
//                        //---coneIndicator----------------------------------------------------------
//                        mCone = new Cone();
//                        mCone->setFillColor(osg::Vec4f(0,1,0,0.4));
//                        mCone->setRadius(osgEarth::Distance(mCurrentModelSize/2, osgEarth::Units::METERS));
//                        mCone->setHeight(osgEarth::Distance(mCurrentModelSize, osgEarth::Units::METERS));
//                        mCone->setLocalRotation(osg::Quat(osg::PI,osg::Vec3d(1,1,0)));
//                        mCone->setCenter(osg::Vec3d(0,0,-hit.drawable->getBoundingBox().zMax()*3));
//                        simpleModelNode->getGeoTransform()->addChild(mCone);
//                        mCone->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0));
//                        //--------------------------------------------------------------------------
//                    }
                    return simpleModelNode;
                }
            }
        }
    }
    return simpleModelNode;
}

void Model::updateModelDataLabel(std::string name)
{
    if (!mRenderImage) {
        mRenderImage = new QImage(
            LABEL_IMAGE_WIDTH,
            LABEL_IMAGE_HEIGHT,
            QImage::Format_RGBA8888
            );
    }
    if(!mImageLabel.valid())
        mImageLabel = new osg::Image;

    {

        mRenderImage->fill(QColor(Qt::transparent));
        QPainter *painter = new QPainter(mRenderImage);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));
        static const QFont textFont("SourceSansPro", 10, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));


        painter->setPen(Qt::NoPen);
        painter->setBrush(backgroundBrush);
        painter->drawRoundedRect(
            mRenderImage->rect(),
            10,2);


        painter->setPen(textPen);
        painter->setFont(textFont);

        painter->drawText(0, 0, LABEL_IMAGE_WIDTH, 20,
                         Qt::AlignCenter|Qt::AlignVCenter,
                         QString::fromStdString(name));

    *mRenderImage = mRenderImage->mirrored(false, true);

    mImageLabel->setImage(LABEL_IMAGE_WIDTH,
                          LABEL_IMAGE_HEIGHT,
                          1,
                          GL_RGBA,
                          GL_RGBA,
                          GL_UNSIGNED_BYTE,
                          mRenderImage->bits(),
                          osg::Image::AllocationMode::NO_DELETE);
}
}

bool Model::clicked() const
{
    return mClicked;
}
