
import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

MainWindow {


    readonly  property int       _iconSize   : 32
    readonly property int       _margin     : 15
    readonly property int       _radius     : 10
    readonly property color     _colorRec   : "#404040"
    readonly property color     _colorHover : "#FFCC00"
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorIcon  : "#FFFFFF"
    readonly property color     _colorButton: "#55FFFFFF"
    readonly property string    _fontFamily : "Srouce Sans Pro"
    readonly property int       _fontPointSize : 11
    readonly property color     itemColor: "#404040"
    readonly property real      widgetsMargis: 10

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"


    id: wnd
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Hello World")





    onClicked: function() {
        if (wnd.widgetsVisible === true) {
            widgetsShowAnimation.stop();
            widgetsHideAnimation.start();
            wnd.widgetsVisible = false;
        } else {
            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            wnd.widgetsVisible = true;
        }
    }

    property var sideItemsModel: ListModel {


        ListElement {
            title_text: "File"
            icon_url: "qrc:///Resources/File.png"
            side_itemurl: "qrc:/toolbox/Toolbox.qml"
        }

        ListElement {
            title_text: "Settings"
            icon_url: "qrc:///Resources/Settings.png"
            side_itemurl: "qrc:///Settings.qml"
        }

        ListElement {
            title_text: "Toolbox"
            icon_url: "qrc:///Resources/Toolbox.png"
            side_itemurl: "qrc:/toolbox/Toolbox.qml"
        }
        ListElement {
            title_text: "Location"
            icon_url: "qrc:/Resources/location.png"
            side_itemurl: "qrc:/location/Location.qml"
        }
        ListElement {
            title_text: "Info"
            icon_url: "qrc:/Resources/info.png"
            side_itemurl: "qrc:/infotarget/InfoTarget.qml"
        }
    }

    property var toolboxModel: ListModel {

    }
    property var fileModel: ListModel {

    }
    PropertyAnimation {
        id: widgetsShowAnimation
        target: wnd
        property: "widgetsPositionFactor"
        from: wnd.widgetsPositionFactor
        to: 1.0
        duration: 200 * Math.abs(1.0 - wnd.widgetsPositionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: widgetsHideAnimation
        target: wnd
        property: "widgetsPositionFactor"
        from: wnd.widgetsPositionFactor
        to: 0.0
        duration: 200 * Math.abs(wnd.widgetsPositionFactor)

        easing.type: Easing.InQuint
    }

    function menuWidgetClickCallback(index) {
        sideWidget.menuWidgetItemClicked(index);
    }

    Item {

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: statusBar.top
        width: parent.width

        MenuWidget {
            id: menuWidget
            anchors.horizontalCenter: parent.horizontalCenter

            y: (-height - widgetsMargis) + (wnd.widgetsPositionFactor * (height + (widgetsMargis * 2.0)))

            width: implicitWidth
            height: implicitHeight

            itemsModel: sideItemsModel
            clickCallback: function(index) {
                wnd.menuWidgetClickCallback(index);
            }
        }

        SideWidget {
            id: sideWidget
            x:  -(implicitWidth + (widgetsMargis*3)) + (wnd.widgetsPositionFactor * ((implicitWidth * 0.5) + (widgetsMargis*2.0)))
            y: menuWidget.height + (widgetsMargis * 2.0)
            width: implicitWidth + (widgetsMargis * 2)
            height: parent.height - menuWidget.height - (widgetsMargis * 3) - navigationWidget.height

            sideItemsModel: wnd.sideItemsModel

            onSideItemCreated: function(index, item) {
                switch(index) {
                case 0:
                    item.listModel = wnd.fileModel;
                    item.itemClicked.connect(wnd.fileItemClicked);
                    break
                case 2:
                    item.listModel = wnd.toolboxModel;
                    item.itemClicked.connect(wnd.toolboxItemClicked);
                    item.changeCheckable.connect(wnd.toolboxItemCheckedChanged);
                    break;


                case 3:
                    item.goToLocation.connect(wnd.goToLocation);
                    wnd.focalPointLatChanged.connect(function(){item.latitude = wnd.focalPointLat;});
                    wnd.focalPointLongChanged.connect(function(){item.longitude = wnd.focalPointLong;});
                    wnd.focalPointRangeChanged.connect(function(){item.range = wnd.focalPointRange;});
                    wnd.focalPointPitchChanged.connect(function(){item.pitch = wnd.focalPointPitch;});
                    wnd.focalPointHeadChanged.connect(function(){item.head = wnd.focalPointHead;});
                    item.goToView.connect(wnd.travelToViewpoint);

                    break;
                }

                wnd.sideItemCreated(index, item);
            }
        }

        InfoWidget {
            id: infoWidget
            x:  -(600 + (widgetsMargis*3)) + (wnd.widgetsPositionFactor * (300 + (widgetsMargis*2.0)))
            y: menuWidget.height + (widgetsMargis * 2.0)
            width: 600 + (widgetsMargis * 2)
            height: parent.height - menuWidget.height - (widgetsMargis * 3) - navigationWidget.height

        }

        Compass{
            id:compass
            headingAngle: wnd.headingAngle
            anchors.right: parent.right
            anchors.rightMargin: widgetsMargis
            anchors.bottomMargin: widgetsMargis
            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargis)))
        }

        NavigationWidget{
            id : navigationWidget
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargis/2)))
            // slot button
            onBtnHomeClicked: function() {
                wnd.homeButtonClicked();
            }
            onBtnProjectionClicked: function() {
                wnd.projectionButtonClicked();
            }
            onBtnZoomInClicked: {
                wnd.zoomInButtonClicked();
            }
            onBtnZoomOutClicked: {
                wnd.zoomOutButtonClicked();
            }
            onBtnUpClicked: {
                wnd.upButtonClicked();
            }
            onBtnDownClicked: {
                wnd.downButtonClicked();
            }
            onBtnLeftClicked: {
                wnd.leftButtonClicked();
            }
            onBtnRightClicked: {
                wnd.rightButtonClicked();
            }
            onBtnRotateLeftClicked:{
                wnd.rotateLeftButtonClicked();
            }
            onBtnRotateRightClicked:{
                wnd.rotateRightButtonClicked();
            }
            onBtnRotateDownClicked: {
                wnd.rotateDownButtonClicked();
            }
            onBtnRotateUpClicked: {
                wnd.rotateUpButtonClicked();
            }
            ///
        }

    }

    function addSideItem(_title_text, _icon_url, _side_itemurl) {
        var new_index = sideItemsModel.count;
        sideItemsModel.append({"title_text": _title_text,
                          "icon_url": _icon_url,
                          "side_itemurl": _side_itemurl});
        return new_index;
    }


    function addToolboxItem(itemDesc) {


        const toolboxModelCount = toolboxModel.count;
        var category_found = false;
        var category_index = -1;
        for (var i = 0; i < toolboxModelCount; i++) {
            if (toolboxModel.get(i).categoryName === itemDesc.category) {
                category_found = true;
                category_index = i;
                break;
            }
        }
        if (category_found == true) {
            const categoryModelCount = toolboxModel.get(category_index).categoryModel.count;
            var itemfound = false;
            for (var j = 0; j < categoryModelCount; j++) {
                if (toolboxModel.get(category_index).categoryModel.get(j).itemName === itemDesc.name) {
                    itemfound = true;
                    break;
                }
            }
            if (itemfound === false) {
                toolboxModel.get(category_index).categoryModel.append({'itemName': itemDesc.name,
                                                  'itemIcon' : itemDesc.iconUrl,
                                                  'itemCheckable' : itemDesc.checkable
                                              });
                wnd.toolboxItemCreated(itemDesc);
            } else {
                return false;
            }
        } else {
            toolboxModel.append({'categoryName': itemDesc.category,
                            'categoryModel': listModelComponent.createObject(null, {})});
            toolboxModel.get(toolboxModelCount).categoryModel.append({'itemName': itemDesc.name,
                                             'itemIcon' : itemDesc.iconUrl,
                                             'itemCheckable' : itemDesc.checkable
                                         });
            wnd.toolboxItemCreated(itemDesc);
        }

        return true;
    }
    function addFileItem(itemDesc) {


        const fileModelCount = fileModel.count;
        var category_found = false;
        var category_index = -1;
        for (var i = 0; i < fileModelCount; i++) {
            if (fileModel.get(i).categoryName === itemDesc.category) {
                category_found = true;
                category_index = i;
                break;
            }
        }
        if (category_found === true) {
            const categoryModelCount = fileModel.get(category_index).categoryModel.count;
            var itemfound = false;
            for (var j = 0; j < categoryModelCount; j++) {
                if (fileModel.get(category_index).categoryModel.get(j).itemName === itemDesc.name) {
                    itemfound = true;
                    break;
                }
            }
            if (itemfound === false) {
                fileModel.get(category_index).categoryModel.append({'itemName': itemDesc.name,
                                                  'itemIcon' : itemDesc.iconUrl,
                                                  'itemCheckable' : itemDesc.checkable
                                              });
                wnd.fileItemCreated(itemDesc);
            } else {
                return false;
            }
        } else {
            fileModel.append({'categoryName': itemDesc.category,
                            'categoryModel': listModelComponent.createObject(null, {})});
            fileModel.get(fileModelCount).categoryModel.append({'itemName': itemDesc.name,
                                             'itemIcon' : itemDesc.iconUrl,
                                             'itemCheckable' : itemDesc.checkable
                                         });
            wnd.fileItemCreated(itemDesc);
        }

        return true;
    }


    function showInfoItem(itemTypeString) {
        infoWidget.showInfoItem(itemTypeString);
    }

    Component {
        id: listModelComponent
        ListModel {
        }
    }

    Component.onCompleted: function() {    }

//    LocationWidget{
//        anchors.left: navigationWidget.right
//        anchors.leftMargin: _margin
//        anchors.right: compass.left
//        anchors.rightMargin: _margin / 2
//        anchors.bottomMargin:  widgetsMargis
//        y : wnd.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargis)))

//        latitude: wnd.mousePointedLocation.x
//        longitude: wnd.mousePointedLocation.y
//        altitude: wnd.mousePointedLocation.z


//    }

    StatusBars {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 25
        latitude: wnd.mousePointedLocation.x
        longitude: wnd.mousePointedLocation.y
        altitude: wnd.mousePointedLocation.z

    }


}
