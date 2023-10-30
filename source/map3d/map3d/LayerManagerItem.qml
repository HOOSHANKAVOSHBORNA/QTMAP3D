import QtQuick.Layouts 1.13
import Crystal 1.0
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects
import "style"

Item {
    id: rootItem
    property var layerModel
    readonly property color backgroundColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)


    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20 / Style.monitorRatio
        Rectangle {
            id:search
            Layout.fillWidth: true
            height: 30 / Style.monitorRatio
            radius: height / 2
            color: backgroundColor

            IconImage {
                id: searchIcon
                source: "qrc:/Resources/search.png"
                width: 24/Style.monitorRatio
                height: 24/Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10/Style.monitorRatio
                color: Style.foregroundColor
            }

            TextField {
                function sendToSearch() {
                    if( rootItem.layerModel){
                        rootItem.layerModel.setFilterString(text)
                        if (text.length === 0) {
                            treeView.collapseRecursively()
                        }
                        treeView.expandRecursively()
                    }}

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: searchIcon.right
                anchors.right: parent.right
                verticalAlignment: Text.AlignVCenter
                font.family: Style.fontFamily
                font.pixelSize: 17/Style.monitorRatio
                color: Style.foregroundColor
                background: Rectangle{
                    color: "transparent"
                    radius: height/2
                }

                onAccepted: {
                    sendToSearch()
                }
                onTextChanged: function() {
                    sendToSearch()
                }
                placeholderText: "Search ..."
                placeholderTextColor: Style.disableColor
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10/Style.monitorRatio
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TreeView{
                id: treeView
                anchors.fill: parent
                clip: true
                rowSpacing: 5 / Style.monitorRatio
                model: layerModel

//                selectionModel: ItemSelectionModel {
//                    id: selectionModel
//                    model: layerModel
//                    onCurrentChanged: {
//                    }
//                    onSelectionChanged:{
//                    }
//                }


                delegate: Item {
                    id: treeDelegate
                    implicitWidth: treeView ? treeView.width ?? 0 : 0
                    implicitHeight:  30/Style.monitorRatio

                    readonly property real indent: 30
                    readonly property real padding: 5
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth

                    Rectangle{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: indent * depth
                        width: 2/Style.monitorRatio
                        height: parent.height + (treeView ? treeView.rowSpacing ?? 0 : 0)
                        visible: depth
                        color: Style.foregroundColor
                        opacity: 0.2
                    }

                    DropArea{
                        anchors.fill: parent
                        id:dropArea
                        visible: dropRole
                        onDropped: {

                            if((treeView.index(row , column) !== layerModel.dragIndex) && (layerModel.dragIndex.valid)){
                                if(layerModel.dragIndex.parent === treeView.index(row , column).parent){
                                    layerModel.onMoveItem(layerModel.dragIndex, treeView.index(row , column))
                                }
                            }
                            treeView.forceLayout()
                        }

                        Rectangle{
                            id: dropEffect
                            anchors.fill: parent
                            gradient: Gradient{
                                orientation: Qt.Horizontal
                                GradientStop{color: "transparent" ; position: 0.0}
                                GradientStop{id:dropEffectColor;color: Style.disableColor ; position: 0.5 }
                                GradientStop{color: "transparent" ; position: 1.0}
                            }
                            width: container.width
                            height: 1
                            y:0
                            x: container.x
                            z:5
                            visible: parent.containsDrag
                        }
                    }


                    Drag.dragType: Drag.Automatic
                    Drag.onDragStarted: {

                        rootItem.layerModel.dragIndex = treeView.index(row, column)

                    }
                    Drag.onDragFinished: {
                        Drag.drop();
                    }

                    Binding on Drag.active {
                        value: dragArea.drag.active
                        delayed: true
                    }

                    MouseArea{
                        id:dragArea
                        anchors.fill: parent
                        drag.target: parent
                        drag.axis: Drag.YAxis
                        propagateComposedEvents: true
                        onClicked: function(mouse){
                            treeView.toggleExpanded(row)
                        }

                    }


                    Rectangle{
                        id: container
                        anchors.fill: parent
                        anchors.leftMargin: depth?(indent) * depth + 10/Style.monitorRatio: 0
                        color: (treeDelegate.hasChildren && expanded)? backgroundColor: "transparent"
                        radius: height/2
                        MouseArea{
                            id: containerMouse
                            propagateComposedEvents: true

                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton;
                            onClicked:function(mouse) {
                                contextMenu.popup()
                            }
                        }



                        IconImage {
                            id: indicator
                            source: "qrc:/Resources/down.png"
                            width: container.height * 0.4 - 2*depth
                            height:container.height * 0.5 - 2*depth
                            color: visibleRole ?  Style.foregroundColor :Style.disableColor
                            anchors.rightMargin: 15/Style.monitorRatio
                            anchors.right:  parent.right
                            visible: treeDelegate.hasChildren
                            rotation: treeDelegate.expanded ? 180 : 0
                            anchors.verticalCenter: parent.verticalCenter

                        }

                        IconImage {
                            id: itemIcon
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin:10/Style.monitorRatio
                            source: /*imageSource ??*/ ""
                            width: 24/Style.monitorRatio
                            height: 24/Style.monitorRatio
                            color:Style.foregroundColor
                        }

                        Text {
                            id: label
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: itemIcon.right
                            anchors.leftMargin: 10/Style.monitorRatio
                            clip: true
                            font.pixelSize: 17/Style.monitorRatio
                            font.weight: Font.Medium
                            color: Style.foregroundColor
                            text: display
                        }

                    }

                    Rectangle{
                        id: selectRect
                        anchors.fill: parent
                        anchors.leftMargin: depth?(indent) * depth + 10/Style.monitorRatio: 0
                        radius: height/2
                        visible: true
                        color: selectRectContainar.containsMouse || hideBtn.containsMouse ? backgroundColor : "transparent"
                        z: -1
                    }
                    MouseArea {
                        id: selectRectContainar
                        anchors.fill: container
                        hoverEnabled: true
                        propagateComposedEvents: true
                        onPressed: function(mouse) {
                            mouse.accepted = false
                        }
                    }

                    Rectangle{
                        id:backgroundbtn
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin:depth?(indent) * depth + 10/Style.monitorRatio: 0
                        width: 28/Style.monitorRatio
                        height: 28/Style.monitorRatio
                        radius: height/2
                        color: hideBtn.containsMouse ? Style.disableColor : "transparent"
                        visible: true
                        opacity: .2

                    }
                    Rectangle{
                        id: hideContainer
                        width: container.height * 0.5
                        height: container.height* 0.5
                        color: "transparent"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left:  container.left
                        anchors.margins: 5
                        IconImage {
                            id:eye
                            source:  "./Resources/48/power.png"
                            width: parent.width
                            height: parent.height
                            anchors.centerIn: parent
                            color: visibleRole ?  Style.hoverColor :Style.disableColor


                        }
                        MouseArea{
                            id:hideBtn
                            hoverEnabled: true
                            anchors.fill: hideContainer
                            propagateComposedEvents: true
                            onClicked:{
                                rootItem.layerModel.onVisibleItemClicked(treeView.index(row , column))
                            }

                        }
                        Menu {
                            id: contextMenu
                            MenuSeparator {
                                contentItem: Rectangle {
                                    implicitWidth: 200/Style.monitorRatio
                                    implicitHeight: 7/Style.monitorRatio
                                    color: "transparent"
                                }
                            }
                            Action{
                                text: qsTr("Delete Layer")
                                icon.source: "qrc:/Resources/garbage.png"
                                icon.color: "red"
                                onTriggered: function() {
                                    rootItem.layerModel.onRemoveItemClicked(treeView.index(row , column))
                                }
                            }
                            MenuSeparator {
                                contentItem: Rectangle {
                                    implicitWidth: 200/Style.monitorRatio
                                    implicitHeight: 7/Style.monitorRatio
                                    color: "transparent"
                                }
                            }
                            Action {
                                text: "Shift Up"
                                icon.source: "qrc:/Resources/up.png"
                                icon.color: Style._persianGreen
                                onTriggered: {
                                    rootItem.layerModel.onMoveItem(treeView.index(row , column), treeView.index(row-1 , column))
                                }
                            }
                            MenuSeparator {
                                contentItem: Rectangle {
                                    implicitWidth: 200/Style.monitorRatio
                                    implicitHeight: 7/Style.monitorRatio
                                    color: "transparent"
                                }
                            }
                            Action {
                                text: "Shift Down"
                                icon.source: "qrc:/Resources/down.png"
                                icon.color: Style._persianGreen
                                onTriggered: {
                                    rootItem.layerModel.onMoveItem(treeView.index(row , column), treeView.index(row+1 , column))
                                }
                            }
                            MenuSeparator {
                                contentItem: Rectangle {
                                    implicitWidth: 200/Style.monitorRatio
                                    implicitHeight: 7/Style.monitorRatio
                                    color: "transparent"
                                }
                            }
                            delegate: MenuItem {
                                id: menuItem
                                implicitWidth: 121/Style.monitorRatio
                                implicitHeight: 22/Style.monitorRatio



                                indicator:IconImage {
                                    source: menuItem.icon.source
                                    width: 22/Style.monitorRatio
                                    height: 22/Style.monitorRatio
                                    color:Style.foregroundColor
                                    anchors.verticalCenter:parent.verticalCenter
                                    x: 10 /Style.monitorRatio

                                }

                                contentItem: Text {
                                    id:rightClickText
                                    leftPadding: menuItem.indicator.width +  menuItem.indicator.x
                                    text: menuItem.text
                                    font.family: Style.fontFamily
                                    font.pixelSize: 17/Style.monitorRatio
                                    font.weight: Font.Medium
                                    opacity: enabled ? 1.0 : 0.3
                                    color: Style.foregroundColor
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                }

                                background: Rectangle {
                                    width: 140/Style.monitorRatio
                                    height: 30/Style.monitorRatio
                                    anchors.centerIn: parent
                                    radius: 10/Style.monitorRatio
                                    opacity: enabled ? 1 : 0.3
                                    color: menuItem.highlighted ? backgroundColor : "transparent"
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 150/Style.monitorRatio
                                implicitHeight: 100/Style.monitorRatio
                                color: Style.backgroundColor
                                radius: 15/Style.monitorRatio
                                opacity: .85


                            }
                        }
                    }
                }
            }
        }

    }

}
