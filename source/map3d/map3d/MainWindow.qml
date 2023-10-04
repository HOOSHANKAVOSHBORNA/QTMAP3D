import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects
import Crystal 1.0
import "style"

CMainWindow {
    id: mainWindow
    visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("MAP3D")

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    readonly property color     _colorHover : "#01AED6"
    readonly property color     _colorPresed : "#003569"


    property Component dockableItemComp: Qt.createComponent("DockableItem.qml");

    function addToCenterCenterContainer(item) {
        centerCenterContainer.data.push(item)
    }

    function addToLeftContainer(item, name) {
        leftContainer.model.append({item:item, name:name})
    }

    function removeFromLeftContainer(item) {
        var indx = -1
        for (var i = 0; i < leftContainer.model.count; ++i){
            if (leftContainer.model.get(i).item === item){
                indx = i
                break
            }
        }
        if (indx > -1)
            leftContainer.model.remove(indx)
    }

    Item {
        id: pinHandler

        state: "unpin"
        states: [
            State {
                name: "unpin"
                ParentChange { target: container; parent: unpinContainer }
            },
            State {
                name: "pin"
                ParentChange { target: container; parent: pinContainer }
            }
        ]
    }

    Item {
        id: unpinContainer

        height: mainWindow.height - 20-2*y
        width: 80/Style.monitorRatio + leftContainer.implicitWidth
        x: 3
        y: 3
        z: 1

        Rectangle {
            id:container

            anchors.fill: parent
            radius: 10

            gradient: Gradient{
             GradientStop { position: 0.0; color: Style.topGradient }
             GradientStop { position: 1.0; color: Style.bottomGradient }
            }
            border.color: Style.borderColor
            border.width: 2
            RowLayout {
                anchors.fill: parent
                Rectangle {
                    id: toolBar
                    Layout.leftMargin: 3
                    Layout.preferredWidth: 80/Style.monitorRatio - 6
                    Layout.preferredHeight: parent.height -5
                    color: Style.backgroundColor
                    border.color: "gray"
                    border.width: 3
                    radius: 10
                    ColumnLayout{
                        id: centerItemUp
                        anchors.fill: parent
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        spacing: 0
                        Button {
                            width: 20
                            icon {
                                source: "qrc:/Resources/toolbox.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }
                            background: Rectangle {
                                color: "transparent"
                            }

                            display: AbstractButton.IconOnly
                            checkable: true
                            checked: false
                            Material.foreground: "white"
                            property var toolboxItem
                            onReleased: {
                                if (checked && mainWindow.toolbox ) {
                                    var toolboxx = Qt.createComponent("ToolboxView.qml");
                                    if (toolboxx.status === Component.Ready) {
                                        toolboxItem = toolboxx.createObject(null, {});
                                        toolboxItem.listModel = mainWindow.toolbox
                                        mainWindow.addToLeftContainer(toolboxItem, "Toolbox")
                                    }
                                    else{
                                        print("can not load toolbox.");
                                    }
                                }
                                else {
                                    removeFromLeftContainer(toolboxItem)
                                }
                            }
                        }
                        Button {
                            width: 28
                            icon {
                                source: "qrc:/Resources/location.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }

                            background: Rectangle { color: "transparent" }

                            display: AbstractButton.IconOnly

                            Material.foreground: "white"

                            checkable: true
                            checked: false
                            property var locationManagerItem
                            onReleased: {
                                if (checked && mainWindow.toolbox) {
                                    var locationManager = Qt.createComponent("LocationManager.qml");
                                    if (locationManager.status === Component.Ready) {
                                        locationManagerItem = locationManager.createObject(null, {});
                                        locationManagerItem.listModel = mainWindow.locationManagerProxyModel
                                        mainWindow.addToLeftContainer(locationManagerItem, "Location Manager")
                                    } else {
                                        print("can not load LocationManager.qml.");
                                    }
                                } else {
                                    removeFromLeftContainer(locationManagerItem)
                                }
                            }
                        }

                        Button {
                            width: 28
                            display: AbstractButton.TextUnderIcon
                            icon {
                                source: "qrc:/Resources/settings.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }
                            background: Rectangle {
                                color: "transparent"
                            }

                            Material.foreground: "white"
                        }
                        Button {
                            id: layerButton
                            width: 28
                            icon {
                                source: "qrc:/Resources/layers.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            display: AbstractButton.TextUnderIcon
                            checkable: true
                            checked: false
                            Material.foreground: "white"
                            property var layerItem
                            onReleased: {
                                if (checked && mainWindow.layersModel ) {
                                    var layersWidget = Qt.createComponent("LayersWidget.qml");
                                    if (layersWidget.status === Component.Ready) {
                                        layerItem = layersWidget.createObject(null, {});
                                        layerItem.layersModell = mainWindow.layersModel
                                        mainWindow.addToLeftContainer(layerItem, "Layers")
                                    } else {
                                        print("can not load Layer Widget")
                                    }
                                } else {
                                    removeFromLeftContainer(layerItem)
                                }
                            }
                        }
                        Button {
                            width: 28
                            icon {
                                source: "qrc:/Resources/list.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            display: AbstractButton.TextUnderIcon

                            Material.foreground: "white"
                            onReleased: {
                                mainWindow.showListWindow()
                            }
                        }

                        Button {
                            width: 28
                            icon {
                                source: "qrc:/Resources/hand.png"
                                width: 28
                                height: 28
                                color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                  (pressed ? _colorHover : _colorPresed);
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            display: AbstractButton.TextUnderIcon

                            Material.foreground: "white"
                            onReleased: {
                                if (pinHandler.state === "pin") {
                                    pinHandler.state = "unpin"
                                } else {
                                    pinHandler.state = "pin"
                                }
                            }
                        }
                        Item {
                            // spacer item
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Rectangle { anchors.fill: parent; color: "transparent" } // to visualize the spacer
                        }
                    }
                }

                //-------------------------------------left dock-----------------
                SideContainer {
                    id: leftContainer
                    Layout.fillWidth: true
                    //                Layout.preferredWidth: visibleCount > 0 ?  implicitWidth : 0
                    //                Layout.maximumWidth: visibleCount > 0 ?  parent.width/3.5 : 0
                }
            }
        }
    }


    SplitView {
        id: mainWindowSplitter
        anchors.fill: parent

        Rectangle {
            id: pinContainer

            visible: pinHandler.state === "pin"
            color: "tomato"
            SplitView.preferredHeight: mainWindow.height - 20-2*y
            SplitView.preferredWidth: 80/Style.monitorRatio + leftContainer.implicitWidth
            x: 3
            y: 3
            z: 1
        }

        StackLayout {
            id: centerCenterContainer
            SplitView.fillWidth: true
            SplitView.fillHeight: true
        }
    }

    ContextmenuWidget {
        id: contextmenu
    }

    function showInfoView(item, title) {
        var indx = -1
        for (var i = 0; i < leftContainer.model.count; ++i){
            if (leftContainer.model.get(i).item === item){
                leftContainer.setCurrentIndex(i)
                return
            }
        }
        leftContainer.model.append({item:item, name:title})
    }

    //---------------------for frame---------------------
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: Style.marginSize
        anchors.topMargin: Style.marginSize
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
                        font.family: Style.fontFamily
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
                        mainWindow.close();
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
                        font.family: Style.fontFamily
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
                        mainWindow.showMinimized()
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
