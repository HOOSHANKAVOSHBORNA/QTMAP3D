
import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects
import Crystal 1.0


CMainWindow {
    readonly property int       _iconSize   : 24
    readonly property int       _margin     : 15
    readonly property int       _radius     : 10
    readonly property color     _colorRec   : "#404040"
    readonly property color     _colorHover : "#FFCC00"
    readonly property color     _colorPresed : "#0dc2df"
    readonly property color     _colorIcon  : "#FFFFFF"
    readonly property color     _colorButton: "#55FFFFFF"
    readonly property string    _fontFamily : "Srouce Sans Pro"
    readonly property int       _fontPointSize : 11
    readonly property color     itemColor: "#404040"
    readonly property real      widgetsMargins: 10

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    property bool init
    property var layerDock
    property var layeritem
    property Component layersWidget
    Component.onCompleted: function() {
//        print(layersModel.columnCount())
//        var layeritem = wnd.layersWidget.createObject(wnd, {layersModell: layersModel});
//        layerDock = wnd.wrapItemWithDockableImpl(layeritem, "Layers");
//        var layeritem = wnd.layersWidget.createObject(wnd, {layersModell: layersModel});
//        var layerDock = wnd.wrapItemWithDockableImpl(layeritem, "Layers");
//        wnd.attachToCentralDockItemImpl(layerDock, true, false, 0.3);
    }

    id: wnd
    visible: true
    width: 800
//    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("MAP3D")
//    MapControllerItem {
//        id: mapController
//        anchors.fill: parent
//        objectName: "MainMap"
//        z: -1
//    }
    property Component dockableItemComp: Qt.createComponent("DockableItem.qml");
    property var toolboxDock
    property var toolboxx
    property DockableItem dockItem: null
    property DockArea defaultDockArea: mainDockArea
    function setCentralDockItemImpl(item) {
        wnd.defaultDockArea.setDefaultDockableItemIfIsDefault(item);
    }

    function attachToCentralDockItemImpl(item, horizontalAttach, attachAsFirst, splitScale) {
        wnd.defaultDockArea.attachDockItemIfIsDefault(horizontalAttach, attachAsFirst, item, splitScale);

    }

    function wrapItemWithDockableImpl(_item, _title) {
        var obj = wnd.dockableItemComp.createObject(wnd, {attachWindow: wnd, tmpColor:"#104020", title: _title});
        obj.wrapItem(_item);
        return obj;
    }


    function startTabDrag(item) {
        wnd.dockItem = item;
    }

    function finishTabDrag() {
        wnd.dockItem = null;
    }
    DockArea     {
        id: mainDockArea
        anchors.fill: parent
        parentMainWindow: wnd
        isDefaultDockArea: true

        Component.onCompleted: function() {
//            var obj = wnd.dockableItemComp.createObject(wnd, {x: 300, y: 300, attachWindow: wnd, tmpColor:"#104020", title: "def"});
//            wnd.defaultDockArea.setDefaultDockableItemIfIsDefault(obj);

//            var dock1 = wnd.dockableItemComp.createObject(wnd, {x: 100, y: 100, attachWindow: wnd, tmpColor:"#102040", title: "First"});
//            wnd.defaultDockArea.attachDockItemIfIsDefault(true, true, dock1, 0.25);

//            var dock2 = wnd.dockableItemComp.createObject(wnd, {x: 100, y: 100, attachWindow: wnd, tmpColor:"#402010", title: "Second"});
//            wnd.defaultDockArea.attachDockItemIfIsDefault(true, false, dock2, 0.35);
        }
    }

    //flags: Qt.FramelessWindowHint
    //    MouseArea{
    //        anchors.fill : parent
    //        property variant clickPos: "1,1"
    //        onPressed: {
    //            clickPos = Qt.point(mouse.x ,mouse.y)
    //        }
    //        onPositionChanged: {
    //            var delta = Qt.point(mouse.x - clickPos.x , mouse.y - clickPos.y)
    //            wnd.x += delta.x
    //            wnd.y += delta.y
    //        }
    //        onDoubleClicked: {
    //            wnd.showFullScreen();
    //        }
    //    }
    //    BorderImage {
    //        id: borderImage
    //        anchors.fill: parent
    //        anchors.bottomMargin: -10
    //        source: "qrc:/Resources/mainFrame.png"
    //        z:1
    //}

//    onClicked: function() {
//        toggleWidgetsVisible();
//    }

//    onWidthChanged: leftContainerHolder.adjustRightContainer(wnd.width);

//    SearchBar {
//        anchors.top: parent.top
//        anchors.topMargin: widgetsMargins
//        anchors.rightMargin: /*fpsLabel.width +*/ widgetsMargins*3
//        anchors.right: parent.right
//    }

    property var sideItemsModel: ListModel {


        ListElement {
            title_text:   "File"
            icon_url:     "qrc:///Resources/File.png"
            side_itemurl: "qrc:/toolbox/Toolbox.qml"
        }



        ListElement {
            title_text:   "Toolbox"
            icon_url:     "qrc:///Resources/Toolbox.png"
            side_itemurl: "qrc:/ToolboxView.qml"
        }

        ListElement {
            title_text:   "Location"
            icon_url:     "qrc:/Resources/location.png"
            side_itemurl: "qrc:/location/Location.qml"
        }

        ListElement {
            title_text:   "Settings"
            icon_url:     "qrc:///Resources/Settings.png"
            side_itemurl: "qrc:/setting/Settings.qml"
        }

        //        ListElement {
        //            title_text:   "Info"
        //            icon_url:     "qrc:/Resources/info.png"
        //            side_itemurl: "qrc:/infotarget/InfoTarget.qml"
        //        }
    }

    property var toolboxModel: toolbox
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
//        anchors.bottom: statusBar.top
        width: parent.width

        MenuWidget {
            id: menuWidget
            anchors.horizontalCenter: parent.horizontalCenter

            //y:15 +  parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/5)+25))
            //menuWidget.height /*(-height - widgetsMargins)*/ + (wnd.widgetsPositionFactor * (height + (widgetsMargins * 2.0)))
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

            onLayersButtonClicked: function() {
//                leftContainerHolder.menuLayersButtonClicked();
//                if (layersModel && !init) {
//                    if (wnd.layersWidget.status === Component.Ready) {
//                        print(layersModel.rowCount())
//                        wnd.layeritem = wnd.layersWidget.createObject(wnd, {layersModell: layersModel});
//                        wnd.layerDock = wnd.wrapItemWithDockableImpl(wnd.layeritem, "Layers");
//                        init = true
//                    }
//                }
//                print(layersModel.columnCount())
//                wnd.attachToCentralDockItemImpl(wnd.layerDock, true, false, 0.3);
                if (!init) {
                    layersWidget = Qt.createComponent("LayersWidget.qml");
                    if (layersWidget.status == Component.Ready && layersModel) {
                        var layeritem = wnd.layersWidget.createObject(null, {});
                        layeritem.layersModell = layersModel
                        var layerDock = wnd.wrapItemWithDockableImpl(layeritem, "Layers");
                        init= true
                    }
//                    if (layeritem.status == Component.Ready)
//                        print(layeritem.errorString)
                    wnd.attachToCentralDockItemImpl(layerDock, true, false, 0.3);
//                } else{
//                    layeritem.destroyCalled = true
//                    init = false
                }


            }
            onToolboxItemClicked: function() {
                toolboxAnimation.start();
                print("ddf")
            }
        }

        SideWidget {
            id: sideWidget
//            x:  -(implicitWidth + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * ((implicitWidth * 0.5) + (widgetsMargins*3)))
//            y: menuWidget.height *2.5 + (widgetsMargins * 2.0)
            x:  -(implicitWidth + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * ((implicitWidth * 0.5) + (widgetsMargins*2.0)))
            y: menuWidget.height + (widgetsMargins * 2.0)
            width: implicitWidth + (widgetsMargins * 2)
            height: wnd.height -  (menuWidget.height *2.5)  - (widgetsMargins * 5) //menuWidget.height - (widgetsMargins * 3) -
//            height: parent.height
            sideItemsModel: wnd.sideItemsModel

            onSideItemCreated: function(index, item) {
                print(item);
                switch(index) {
                case 0:
                    item.listModel = wnd.toolboxModel;
                    item.itemClicked.connect(wnd.fileItemClicked);
                    break
                case 1:
                    toolboxx = item
                    toolboxx.listModel = wnd.toolboxModel;
                    toolboxDock = wnd.wrapItemWithDockableImpl(toolboxx, "Toobox");
                    wnd.attachToCentralDockItemImpl(toolboxDock, true, true, 0.2);
//                    item.itemClicked.connect(wnd.toolboxItemClicked);
//                    item.changeCheckable.connect(wnd.toolboxItemCheckedChanged);
                    break;


                case 2:
//                    item.goToLocation.connect(wnd.goToLocation);
//                    wnd.focalPointLatChanged.connect(function(){item.latitude = wnd.focalPointLat;});
//                    wnd.focalPointLongChanged.connect(function(){item.longitude = wnd.focalPointLong;});
//                    wnd.focalPointRangeChanged.connect(function(){item.range = wnd.focalPointRange;});
//                    wnd.focalPointPitchChanged.connect(function(){item.pitch = wnd.focalPointPitch;});
//                    wnd.focalPointHeadChanged.connect(function(){item.head = wnd.focalPointHead;});
//                    item.goToView.connect(wnd.travelToViewpoint);

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

//        Compass{
//            id:compass
//            headingAngle: wnd.headingAngle
//            anchors.left: parent.left
//            anchors.leftMargin: widgetsMargins //+ 10
//            anchors.bottomMargin: widgetsMargins
////            y: 25 + parent.height - (wnd.widgetsPositionFactor * (height + (widgetsMargins - 10)+ 25) )
//            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargins)))

//            onCompassDoubleClicked: function() {
//                wnd.orientCameraToNorth();
//            }
//        }


//        NavigationWidget{
//            id : navigationWidget
//            anchors.right: parent.right
//            anchors.rightMargin: _margin
////            y:25 + parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/2+3)+25))
//            y: parent.height  - (wnd.widgetsPositionFactor * (height + (widgetsMargins/2+3)))
//            // slot button
//            onBtnHomeClicked: function() {
//                wnd.homeButtonClicked();
//            }
//            onBtnProjectionClicked: function() {
//                wnd.view3DButtonClicked();
//            }
//        }
    }


    Component {
        id: listModelComponent
        ListModel {
        }
    }


//    StatusBar {
//        id: statusBar
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 0
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.leftMargin: 0
//        anchors.rightMargin: 0
//        width: parent.width
//        height: childrenRect.height
//        latitude: wnd.mousePointingLocationWgs84.x
//        longitude: wnd.mousePointingLocationWgs84.y
//        altitude: wnd.mousePointingLocationWgs84.z
//        coordinate1: wnd.mousePointingLocation.x
//        coordinate2: wnd.mousePointingLocation.y
//        coordinate3: wnd.mousePointingLocation.z
//        message: "Ready"
//        timer: -1

//    }


//    Label {
//        id: fpsLabel
//        text: wnd.fps.toLocaleString(Qt.locale(), 'f', 2)
//        color: 'springgreen'
//        style: Text.Outline;
//        styleColor: "black"
//        font.pointSize: 20
//        font.weight: Font.Bold
//        anchors.right: parent.right
//        anchors.top: parent.top
//        anchors.topMargin: 20
//        anchors.rightMargin: 20
//    }
    ContextmenuWidget {
        id: contextmenu
    }


    function menuWidgetClickCallback(index) {
        infoo.hideItem();
        sideWidget.menuWidgetItemClicked(index);
        if (index == 1){
            toolboxx.visible = !toolboxx.visible
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
        print(itemDesc.name)
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
//            infoo.hideItem()


        } else {

            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            wnd.widgetsVisible = true;
        }
    }
    InformationView {
        id: infoo
//        x:  -(600 + (widgetsMargins*3)) + (wnd.widgetsPositionFactor * (300 + (widgetsMargins*3.0)))
//        y: menuWidget.height *2.5 + (widgetsMargins * 2.0)
        x:  -(600 + (widgetsMargins*4)) + (wnd.widgetsPositionFactor * (300 + (widgetsMargins*3.0)))
        y: menuWidget.height + (widgetsMargins * 2.0)
        width: 600 + (widgetsMargins * 2)
//        height: parent.height - (menuWidget.height *2.5)- navigationWidget.height - (widgetsMargins * 5)
        height: parent.height - menuWidget.height - (widgetsMargins * 3) /*- navigationWidget.height*/
    }

    function showInfoView(item, title) {
//        if (wnd.widgetsVisible === false) toggleWidgetsVisible();
//        sideWidget.hideAllItems();
//        infoo.showInfo(item)
//        infoo.titleText = title;
        var dock = wnd.wrapItemWithDockable(item, "title");
        wnd.attachToCentralDockItem(dock, true, true, 0.2);
    }

    function hideInfoView() {
        infoo.hideItem();
    }


    RightContainer {
        id: leftContainerHolder
        x: parent.width + widgetsMargins*2 - (wnd.widgetsPositionFactor * (250 + (widgetsMargins *3.0))) +300
        y: menuWidget.height + (widgetsMargins*2 )
        width: 250
        height: parent.height - menuWidget.height - (widgetsMargins * 6) /*- navigationWidget.height*/
//        m: layersModel
        }

    function showRightContainer(item) {
        if (wnd.widgetsVisible === false) toggleWidgetsVisible();
        leftContainerHolder.showProp(item);
    }

    function hideRightContainer(item) {
        leftContainerHolder.hideProp(item);
    }

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: _margin
        anchors.topMargin: widgetsMargins
        height: rowl.impliitemClickedcitHeight + 15
        width: rowl.implicitWidth + 20
        visible: false
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "#404040"
            opacity: 0.8
            radius: 10
        }
        MouseArea {
            anchors.fill: parent
        }

        RowLayout {
            id: rowl
            anchors.fill: parent
            spacing: 0
            anchors.rightMargin: 20
            Item {
                Layout.alignment: Qt.AlignCenter

                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col3
                    anchors.centerIn: parent
                    Image {
                        id: img3
                      Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:///Resources/close.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "Exit"
                        color: "white"
                        font.family: _fontFamily
                        font.pointSize: 11
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea3
                    anchors.fill: col3
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        wnd.close();
                        //rootItem.clickCallback(index);
                    }
                }

                MultiEffect {
                    anchors.fill: col3
                    source: col3
                    colorization: 1
                    colorizationColor: mouseArea3.isMouseOnItem ?
                               (mouseArea3.pressed ? _colorPresed: _colorHover) :
                               (mouseArea3.pressed ? _colorHover : "#FFFFFF");
                }
            }
            Item {
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col4
                    anchors.centerIn: parent
                    Image {
                        id: img4
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:///Resources/collapse.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "Mininnize"
                        color: "white"
                        font.family: _fontFamily
                        font.pointSize: 11
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea4
                    anchors.fill: col4
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        wnd.showMinimized()
                        //rootItem.clickCallback(index);
                    }
                }

                MultiEffect {
                    anchors.fill: col4
                    source: col4
                    colorization: 1
                    colorizationColor: mouseArea4.isMouseOnItem ?
                               (mouseArea4.pressed ? _colorPresed: _colorHover) :
                               (mouseArea4.pressed ? _colorHover : "#FFFFFF");
                }
            }
        }
    }
}
