
import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

CMainWindow {

    readonly  property int      _iconSize   : 24
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
    readonly property real      widgetsMargins: 10

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"


    zoomInButtonPressed:      navigationWidget.zoomInButtonPressed
    zoomOutButtonPressed:     navigationWidget.zoomOutButtonPressed
    upButtonPressed:          navigationWidget.upButtonPressed
    downButtonPressed:        navigationWidget.downButtonPressed
    leftButtonPressed:        navigationWidget.leftButtonPressed
    rightButtonPressed:       navigationWidget.rightButtonPressed
    rotateUpButtonPressed:    navigationWidget.rotateUpButtonPressed
    rotateDownButtonPressed:  navigationWidget.rotateDownButtonPressed
    rotateLeftButtonPressed:  navigationWidget.rotateLeftButtonPressed
    rotateRightButtonPressed: navigationWidget.rotateRightButtonPressed


    id: wnd
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("MAP3D")



    onClicked: function() {
        toggleWidgetsVisible();
    }

    property var sideItemsModel: ListModel {


        ListElement {
            title_text:   "File"
            icon_url:     "qrc:///Resources/File.png"
            side_itemurl: "qrc:/toolbox/Toolbox.qml"
        }



        ListElement {
            title_text:   "Toolbox"
            icon_url:     "qrc:///Resources/Toolbox.png"
            side_itemurl: "qrc:/toolbox/Toolbox.qml"
        }

        ListElement {
            title_text:   "Location"
            icon_url:     "qrc:/Resources/location.png"
            side_itemurl: "qrc:/location/Location.qml"
        }

        ListElement {
            title_text:   "Settings"
            icon_url:     "qrc:///Resources/Settings.png"
            side_itemurl: "qrc:///Settings.qml"
        }

        //        ListElement {
        //            title_text:   "Info"
        //            icon_url:     "qrc:/Resources/info.png"
        //            side_itemurl: "qrc:/infotarget/InfoTarget.qml"
        //        }
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

    Item {

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: statusBar.top
        width: parent.width

        MenuWidget {
            id: menuWidget
            anchors.horizontalCenter: parent.horizontalCenter

            y: (-height - widgetsMargins) + (wnd.widgetsPositionFactor * (height + (widgetsMargins * 2.0)))

            width: implicitWidth
            height: implicitHeight

            itemsModel: sideItemsModel
            clickCallback: function(index) {
                wnd.menuWidgetClickCallback(index);
            }


            onListsButtonClicked: function() {
                wnd.showListWindow();
            }
        }

        SideWidget {
            id: sideWidget
            x:  -(implicitWidth + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * ((implicitWidth * 0.5) + (widgetsMargins*2.0)))
            y: menuWidget.height + (widgetsMargins * 2.0)
            width: implicitWidth + (widgetsMargins * 2)
            height: parent.height - menuWidget.height - (widgetsMargins * 3) - navigationWidget.height

            sideItemsModel: wnd.sideItemsModel

            onSideItemCreated: function(index, item) {
                switch(index) {
                case 0:
                    item.listModel = wnd.fileModel;
                    item.itemClicked.connect(wnd.fileItemClicked);
                    break
                case 1:
                    item.listModel = wnd.toolboxModel;
                    item.itemClicked.connect(wnd.toolboxItemClicked);
                    item.changeCheckable.connect(wnd.toolboxItemCheckedChanged);
                    break;


                case 2:
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

//        InfoWidget {
//            id: infoWidget
//            x:  -(600 + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * (300 + (widgetsMargins*2.0)))
//            y: menuWidget.height + (widgetsMargins * 2.0)
//            width: 600 + (widgetsMargins * 2)
//            height: parent.height - menuWidget.height - (widgetsMargins * 3) - navigationWidget.height

//            onView2D3DButtonClicked: function() {
//                wnd.infoWidget2D3DButtonClicked();
//            }

//            onRouteButtonClicked: function() {
//                wnd.infoWidgetRouteButtonClicked();
//            }

//            onFollowButtonClicked: function() {
//                wnd.infoWidgetFollowButtonClicked();
//            }

//            onMoreButtonClicked: function() {
//                wnd.infoWidgetMoreButtonClicked();
//            }

//        }

        Compass{
            id:compass
            headingAngle: wnd.headingAngle
            anchors.left: parent.left
            anchors.leftMargin: widgetsMargins
            anchors.bottomMargin: widgetsMargins
            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargins)))
        }

        NavigationWidget{
            id : navigationWidget
            anchors.right: parent.right
            anchors.rightMargin: _margin
            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargins/2+3)))
            // slot button
            onBtnHomeClicked: function() {
                wnd.homeButtonClicked();
            }
            onBtnProjectionClicked: function() {
                wnd.view3DButtonClicked();
            }
        }

    }
    Component {
        id: listModelComponent
        ListModel {
        }
    }

    StatusBars {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: childrenRect.height
        latitude: wnd.mousePointingLocationWgs84.x
        longitude: wnd.mousePointingLocationWgs84.y
        altitude: wnd.mousePointingLocationWgs84.z

        coordinate1: wnd.mousePointingLocation.x
        coordinate2: wnd.mousePointingLocation.y
        coordinate3: wnd.mousePointingLocation.z
        message: "Ready"
        timer: -1

    }


    Label {
        id: fpsLabel
        text: wnd.fps.toLocaleString(Qt.locale(), 'f', 2)
        color: 'red'
        font.pointSize: 30
        font.weight: Font.Bold
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.rightMargin: 100
    }
    ContextmenuWidget {
        id: contextmenu
    }


    function menuWidgetClickCallback(index) {
        infoo.hideItem();
        sideWidget.menuWidgetItemClicked(index);
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
        if (category_found === true) {
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


//    function showInfoItem(itemTypeString) {
//        if (wnd.widgetsVisible === false) toggleWidgetsVisible();
//        sideWidget.hideAllItems();
//        infoWidget.showInfoItem(itemTypeString);
//    }
//    function updateInfoWidgetData(infoJson) {
//        infoWidget.updateData(infoJson);
//    }


    function showStatusMessage(message, timer) {
        statusBar.showMessage(message, timer);
    }
    function addContextmenu(item, x, y) {
        contextmenu.showItem(item)
        contextmenu.updatePosition(x, y)
    }
    function hideContextmenu(){
        contextmenu.hideItem()
    }
    function updateXYContextmenu(x, y) {
        contextmenu.updatePosition(x, y)
    }


    function toggleWidgetsVisible() {
        if (wnd.widgetsVisible === true) {
            widgetsShowAnimation.stop();
            widgetsHideAnimation.start();
            wnd.widgetsVisible = false;


        } else {
            sideWidget.hideAllItems();
            infoo.hideItem()

            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            wnd.widgetsVisible = true;
        }

    }
    InformationView {
        id: infoo
        x:  -(600 + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * (300 + (widgetsMargins*2.0)))
        y: menuWidget.height + (widgetsMargins * 2.0)
        width: 600 + (widgetsMargins * 2)
        height: parent.height - menuWidget.height - (widgetsMargins * 3) - navigationWidget.height

    }

    function showInfoView(item) {
        if (wnd.widgetsVisible === false) toggleWidgetsVisible();
        sideWidget.hideAllItems();
        infoo.showInfo(item)
    }

}
