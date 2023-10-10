import QtQuick.Layouts 1.13
import Crystal 1.0
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects
import "style"

Item {
    id: rootItem
    property CLayersModel layersModell
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
                    if( rootItem.layersModell){
                        rootItem.layersModell.setFilterString(text)
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
                rowSpacing: 5/Style.monitorRatio
                model: selectionModel.model

                selectionModel: ItemSelectionModel {
                    id: selectionModel
                    model: layersModell ?? null
                    onCurrentChanged: {
                    }
                    onSelectionChanged:{
                    }
                }


                delegate: Item {
                    id: treeDelegate
                    implicitWidth: treeView.width
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
                        height: parent.height + treeView.rowSpacing
                        visible: depth
                        color: Style.foregroundColor
                        opacity: 0.2
                    }

                    DropArea{
                        anchors.fill: parent
                        id:dropArea
                        visible: dropRole
                        onDropped: {

                            if((treeView.index(row , column) !== layersModell.dragIndex) && (layersModell.dragIndex.valid)){
                                if(layersModell.dragIndex.parent === treeView.index(row , column).parent){
                                    layersModell.onMoveItem(layersModell.dragIndex, treeView.index(row , column))
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

                        rootItem.layersModell.dragIndex = treeView.index(row, column)

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
                        onClicked: {
                            treeView.toggleExpanded(row)
                        }
                    }


                    Rectangle{
                        id: container
                        anchors.fill: parent
                        anchors.leftMargin: depth?(indent) * depth + 10/Style.monitorRatio: 0
                        color: (expanded )? backgroundColor: "transparent"
                        radius: height/2
                        MouseArea{
                            id: containerMouse
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton;
                            onClicked: {
                                contextMenu.popup()

                            }
                        }
                    }

                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                container.color = backgroundColor
                            }else{
                                container.color = "transparent"
                            }
                        }
                    }

                    TapHandler {
                        onPressedChanged: pressed ? label.color = Style.hoverColor : !pressed ? label.color = Style.textColor : Style.textColor
                    }



                    IconImage {
                        id: indicator
                        source: "qrc:/Resources/down.png"
                        width: container.height * 0.4 - 2*depth
                        height:container.height * 0.5 - 2*depth
                        color: visibleRole ?  Style.foregroundColor :Style.disableColor
                        anchors.rightMargin: 15
                        anchors.right:  parent.right
                        visible: treeDelegate.hasChildren
                        rotation: treeDelegate.expanded ? 180 : 0
                        anchors.verticalCenter: parent.verticalCenter

                    }

                    IconImage {
                        id: itemIcon
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin:depth?(indent) * depth + 10/Style.monitorRatio: 0
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
                        color: visibleRole ?  Style.foregroundColor :Style.disableColor
                        text: display
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
                            color: visibleRole ?  Style.foregroundColor :Style.disableColor


                        }
                        MouseArea{
                            id:hideBtn
                            hoverEnabled: true
                            anchors.fill: hideContainer
                            onClicked: function() {
                                rootItem.layersModell.onVisibleItemClicked(treeView.index(row , column))
                            }

                        }
                        Menu {
                            id: contextMenu
                            MenuItem{height: 30
                                text: "Delete Layer"
                                icon.source: "./Resources/48/delete.png"
                                icon.color: "red"
                                onClicked: function() {
                                    rootItem.layersModell.onRemoveItemClicked(treeView.index(row , column))
                                }
                            }
                            MenuItem {
                                text: "Shift Up"
                                icon.source: "./Resources/48/arrow-outline-up.png"
                                icon.color: Style._persianGreen
                                onClicked: {
                                    rootItem.layersModell.onMoveItem(treeView.index(row , column), treeView.index(row-1 , column))
                                }

                            }
                            MenuItem {
                                text: "Shift Down"
                                icon.source: "./Resources/48/arrow-outline-down.png"
                                icon.color: Style._persianGreen
                                onClicked: {
                                    rootItem.layersModell.onMoveItem(treeView.index(row , column), treeView.index(row+1 , column))
                                }
                            }
                        }
                    }
                }
            }
        }

    }

}

