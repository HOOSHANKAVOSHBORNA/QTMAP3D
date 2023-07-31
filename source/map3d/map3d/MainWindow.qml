import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects
import Crystal 1.0
import "style"

CMainWindow {
    id: wnd
    visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("MAP3D")

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"

    property Component dockableItemComp: Qt.createComponent("DockableItem.qml");

    function addToCenterCenterContainer(item) {
        centerCenterContainer.data.push(item)
    }

    function addToLeftContainer(item, name) {
        leftContainer.model.append({item:item, name:name})
    }

    function addToRightContainer(item, name) {
        rightContainer.model.append({item:item, name:name})
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

    function removeFromRightContainer(item) {
        var indx = -1
        for (var i = 0; i < rightContainer.model.count; ++i){
            if (rightContainer.model.get(i).item === item){
                indx = i
                break
            }
        }
        if (indx > -1)
            rightContainer.model.remove(indx)
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: Rectangle {
            MouseArea {
                id: test
                anchors.fill: parent
                hoverEnabled: true

                onPressed: function(mouse) {
                    mouse.accepted = false
                }
                propagateComposedEvents: true
                cursorShape: Qt.SizeHorCursor

            }
            implicitWidth: 4
            height: parent.height
            color: test.containsMouse ? Style.hoverColor : "transparent"
        }

        //-------------------------------------left dock-----------------
        SideContainer {
            id: leftContainer
            SplitView.preferredWidth: visibleCount > 0 ?  implicitWidth : 0
            SplitView.maximumWidth: parent.width/3.5
        }

        SplitView {
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            orientation: Qt.Vertical
            handle: Item {

            }

            RowLayout {
                id: upContainer
                SplitView.maximumHeight: 50
                SplitView.minimumHeight: 50
                SplitView.preferredHeight: 50
                SplitView.fillWidth: true
                Rectangle {
                    id: toolBar
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    color: Style.backgroundColor
                    RowLayout{
                        id: centerItemUp
                        anchors.fill: parent
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        spacing: 0
                        Item {
                            // spacer item
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Rectangle { anchors.fill: parent; color: "transparent" } // to visualize the spacer
                        }
                        Button {
                            width: implicitWidth
                            Layout.fillHeight: true
                            icon {
                                source: "qrc:/Resources/Toolbox.png"
                                width: 21
                                height: 21
                                color: hovered? Style.hoverColor : checked ? Style.selectionColor : "white"

                            }
                            background: Rectangle {
                                color: "transparent"
                            }

                            display: AbstractButton.TextUnderIcon

                            //                            text: "ToolBox"
                            checkable: true
                            checked: false
                            Material.foreground: "white"
                            property var toolboxItem
                            onReleased: {
                                if (checked && wnd.toolbox ) {
                                    var toolboxx = Qt.createComponent("ToolboxView.qml");
                                    if (toolboxx.status === Component.Ready) {
                                        toolboxItem = toolboxx.createObject(null, {});
                                        toolboxItem.listModel = wnd.toolbox
                                        wnd.addToLeftContainer(toolboxItem, "Toolbox")
                                    }
                                }
                                else {
                                    removeFromLeftContainer(toolboxItem)
                                }
                            }
                        }
                        Button {
                            Layout.fillHeight: true
                            width: implicitWidth
                            icon {
                                source: "qrc:/Resources/location.png"
                                width: 21
                                height: 21
                                color: hovered? Style.hoverColor : checked ? Style.selectionColor : "white"
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            display: AbstractButton.TextUnderIcon

                            //                            text: "Location"
                            Material.foreground: "white"
                        }
                        Button {
                            Layout.fillHeight: true
                            width: implicitWidth
                            display: AbstractButton.TextUnderIcon
                            icon {
                                source: "qrc:/Resources/Settings.png"
                                width: 21
                                height: 21
                                color: hovered? Style.hoverColor : checked ? Style.selectionColor : "white"
                            }
                            background: Rectangle {
                                color: "transparent"
                            }

                            //                            text: "Settings"
                            Material.foreground: "white"
                        }
                        Button {
                            id: layerButton
                            Layout.fillHeight: true
                            width: implicitWidth
                            icon {
                                source: "qrc:/Resources/Layers.png"
                                width: 21
                                height: 21
                                color: hovered? Style.hoverColor : checked ? Style.selectionColor : "white"
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
                                if (checked && wnd.layersModel ) {
                                    var layersWidget = Qt.createComponent("LayersWidget.qml");
                                    if (layersWidget.status === Component.Ready) {
                                        layerItem = layersWidget.createObject(wnd, {});
                                        layerItem.layersModell = wnd.layersModel
                                        addToRightContainer(layerItem, "Layers")
                                    }
                                }
                                else {
                                    removeFromRightContainer(layerItem)
                                }
                            }
                        }
                        Button {
                            Layout.fillHeight: true
                            width: implicitWidth
                            icon {
                                source: "qrc:/Resources/Outliner.png"
                                width: 21
                                height: 21
                                color: hovered? Style.hoverColor : checked ? Style.selectionColor : "white"
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            display: AbstractButton.TextUnderIcon

                            //                            text: "List"
                            Material.foreground: "white"
                            onReleased: {
                                wnd.showListWindow()
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
            }

            StackLayout {
                id: centerCenterContainer
                SplitView.fillHeight: true
                SplitView.fillWidth: true
            }
        }
        SideContainer {
            id: rightContainer
            SplitView.preferredWidth: visibleCount > 0 ?  implicitWidth : 0
            SplitView.maximumWidth: parent.width/3.5
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
