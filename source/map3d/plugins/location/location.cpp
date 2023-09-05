#include "location.h"
#include <osgEarth/GLUtils>


using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;


Location::Location(QObject *parent): PluginInterface(parent)
{
}

bool Location::setup()
{

    mLocationLayer = new CompositeAnnotationLayer;
    mLocationLayer->setName(CATEGORY);
    mapItem()->getMapObject()->addLayer(mLocationLayer);

    ///////////////////////////add map pin/////////////////////////////////
    auto mapPinToolBoxItem =  new ToolboxItem{MAPPIN, CATEGORY, "qrc:/resources/mappin.png", true};
    QObject::connect(mapPinToolBoxItem, &ToolboxItem::itemChecked, this, &Location::onMapPinClicked);
    toolbox()->addItem(mapPinToolBoxItem);



    return true;
}



bool Location::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::READY) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            add(geoPos);
            return true;
        }
        if (mState == State::MOVING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            moving(geoPos);
            return true;
        }
    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancelAdd();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        confirm();
        return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void Location::onMapPinClicked(bool check)
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
        mMode = Mode::MapPin;

        delete colorDialog;

    }else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////


void Location::add(const osgEarth::GeoPoint &geoPos)
{


    switch (mMode) {
    case Mode::MapPin:
        mPinMapLayer = new ParenticAnnotationLayer;
        mPinMapLayer->setName("Pin Map");
        mMapPin = new MapPin(mapItem(),mObjectColor);
        mMapPin->setPosition(geoPos);
        mPinMapLayer->addChild(mMapPin);
        mLocationLayer->addLayer(mPinMapLayer);
        break;
    default:
        break;
    }
    mState = State::MOVING;
}

void Location::moving(const osgEarth::GeoPoint &geoPos)
{
    switch (mMode) {
    case Mode::MapPin:
        mMapPin->setPosition(geoPos);
        break;

    default:
        break;
    }
}


void Location::confirm()
{
    if (mState == State::MOVING) {
        mState = State::READY;
    }
}

void Location::cancelAdd(){

    if(mState == State::MOVING){
        switch (mMode) {
        case Mode::MapPin:
            if(!mPinMapLayer->children().isEmpty()){
                mPinMapLayer->removeChild(mMapPin);
                if(mPinMapLayer->children().isEmpty()){
                    mLocationLayer->removeLayer(mPinMapLayer);
                }
            }
            break;

        default:
            break;
        }
        mState = State::READY;
    }
}
