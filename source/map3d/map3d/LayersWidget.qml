import QtQuick.Layouts 1.13
import Crystal 1.0
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects
import "style"




Item {
    id: rootItem
    implicitWidth: 230
    property CLayersModel layersModell
    //    property bool destroyCalled: false

    //    onDestroyCalledChanged: rootItem.destroy()


    Rectangle {
        anchors.fill: parent
        color: Style._darkestGray
        opacity: 1


        Item{
            anchors.horizontalCenter: parent.horizontalCenter
            y : 0
            id:treerootItem
            width: parent.width
            height: parent.height - 40

            Rectangle {
                id: search
                width: parent.width
                height: 40
                y : -1
                clip: true
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                TextField {
                    function sendToSearch() {
                        if( rootItem.layersModell){
                            rootItem.layersModell.setFilterString(text)
                            if (text.length === 0) {
                                treeView.collapseRecursively()
                            }
                            treeView.expandRecursively()
                        }}


                    background: Rectangle {
                        implicitWidth: search.width
                        implicitHeight: 35
                        border.color: "black"
                        border.width: Style.borderwidth
                        color: Style.backgroundColor
                    }
                    anchors.fill: parent
                    color: Style.textColor
                    IconImage{
                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height * 0.6
                        width: height
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        color: Style.textColor
                        source: "./Resources/48/search.png"
                    }

                    placeholderText: "Search Layers"
                    placeholderTextColor: Style.selectionColor
                    onAccepted: {
                        sendToSearch()
                    }
                    onTextChanged: function() {
                        sendToSearch()
                    }

                }
            }




            TreeView{
                id:treeView
                anchors.top: search.bottom
                anchors.topMargin: 7
                width: parent.width -10
                height: parent.height - 35
                anchors.horizontalCenter: parent.horizontalCenter


                model: selectionModel.model
                clip: true

                selectionModel: ItemSelectionModel {
                    id: selectionModel
                    model: layersModell ?? null

                    onCurrentChanged: {

                    }
                    onSelectionChanged:{
                    }
                }
                flickableDirection: Qt.Vertical

                //                DropArea{
                //                    id: dropAre
                //                    anchors.fill: parent
                //                }

                delegate: Item {
                    id: treeDelegate
                    implicitWidth: treerootItem.width
                    implicitHeight: label.implicitHeight * 2 - 4*depth


                    readonly property real indent: 25
                    readonly property real padding: 5
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth

                    property real dropPositionRow


//                    MouseArea {
//                        id: mouseArea
//                        anchors.fill: parent
//                        drag {
//                            target: treeDelegate
//                            axis: Drag.YAxis
//                        }

//                        property bool dragActive: drag.active

//                        onDragActiveChanged: {
//                            if(drag.active) { //
//                                console.log("Active at " + treeView.index(row , column))
//                            } else {
//                                console.log("InActive at " + treeView.index(row , column))
//                            }
//                        }
//                    }



//                    Rectangle{
//                        width: parent.width
//                        height: parent.height
//                        color: Style._darkGray
//                        opacity: 0
//                        z:3
//                        Text {
//                            text: display
//                            anchors.centerIn: parent
//                        }
//                    }

                    DropArea{
                        anchors.fill: parent
                        id:dropArea
                        onEntered: {
                            layersModell.dropIndex = treeView.index(row , column)

                        }
                        onDropped: {
                            console.log("dropped here:" + treeView.index(row , column))
                        }
                    }

                    Drag.active: dragArea.drag.active

                    MouseArea{
                        id:dragArea
                        anchors.fill: parent
                        drag.target: parent
                        drag.axis: Drag.YAxis
                        onPressAndHold: {
                            drag.dragStarted();
                        }

                        onReleased:  {
                            //                            console.log(treeView.index(row , column)+ "onrelease")
                            layersModell.onReplaceItem(treeView.index(row , column))
                            drag.dragFinished()
                        }
                    }


                    Rectangle{
                        id: container
                        width:  parent.width - x - 10
                        height:   parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        color:  Style._darkestGray
                        radius:   Style.radius
                        x:  ((treeDelegate.depth ) * treeDelegate.indent)
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
                                container.color = Style._darkGray
                                label.color = Style.textHoverColor
                            }else{
                                container.color = Style._darkestGray
                                label.color = Style.textColor
                            }
                        }
                    }

                    TapHandler {
                        onTapped: {
                            treeView.toggleExpanded(row)
                        }
                        onPressedChanged: pressed ? label.color = Style.hoverColor : !pressed ? label.color = Style.textColor : Style.textColor
                    }



                    IconImage {
                        id: indicator
                        source: "qrc:/Resources/arrow.png"
                        width: container.height * 0.4 - 2*depth
                        height:container.height * 0.5 - 2*depth

                        anchors.rightMargin: 15
                        //                            anchors.top :parent.top
                        anchors.right:  parent.right
                        visible: treeDelegate.hasChildren
                        rotation: treeDelegate.expanded ? -90 : 0
                        anchors.verticalCenter: parent.verticalCenter

                    }

                    Text {
                        id: label
                        x: treeDelegate.padding + (treeDelegate.isTreeNode ? (treeDelegate.depth) * treeDelegate.indent : 0) + hideContainer.width*1.3
                        //                anchors.left: cont
                        //                width: treeDelegate.width - treeDelegate.padding - x
                        clip: true
                        font.pixelSize: 16 - depth
                        anchors.verticalCenter: container.verticalCenter
                        color: Style.textColor
                        text: display
                        //                anchors.margins: 5
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
                            color: isVisible ?  Style._mainBlue : "red"

                        }
                        MouseArea{
                            id:hideBtn
                            //                    enabled: isVisible
                            hoverEnabled: true
                            anchors.fill: hideContainer
                            onEntered: eye.color = Style._mainYellow
                            onExited: isVisible ? eye.color = Style._mainBlue : eye.color = "red"

                            onClicked: function() {
                                rootItem.layersModell.onItemClicked(treeView.index(row , column))
                            }

                        }
                        Menu {
                            id: contextMenu
                            MenuItem{height: 30
                                text: "Delete Layer"
                                icon.source: "./Resources/48/delete.png"
                                icon.color: "red"
                                onClicked: function() {
                                    rootItem.layersModell.onDeleteLayerClicked(treeView.index(row , column))
                                }
                            }
                            MenuItem {
                                visible: isLocatable
                                height: (isLocatable) ? 30 : 0
                                text: "Show On Map"
                                icon.source: "./Resources/48/location.png"
                                icon.color: Style._mainYellow
                                onClicked: {
                                    rootItem.layersModell.onGoToClicked(treeView.index(row , column))
                                }
                            }
                            MenuItem {
                                text: "Shift Up"
                                icon.source: "./Resources/48/arrow-outline-up.png"
                                icon.color: Style._persianGreen
                                onClicked: {
                                    rootItem.layersModell.dropIndex = treeView.index(row-1 , column)
                                    rootItem.layersModell.onReplaceItem(treeView.index(row , column))
                                }

                            }
                            MenuItem {
                                text: "Shift Down"
                                icon.source: "./Resources/48/arrow-outline-down.png"
                                icon.color: Style._persianGreen
                                onClicked: {
                                    rootItem.layersModell.dropIndex= treeView.index(row+1 , column)
                                    rootItem.layersModell.onReplaceItem(treeView.index(row , column))
                                }
                            }
                        }
                    }
                }

                //                DropArea {
                //                    id: dropAre
                //                    anchors.fill: parent

                //                    onPositionChanged: {
                //                        console.log(drag.source.TreeView.row)
                //                    }


                //                    onEntered: {
                //                        drag.source.Drag.drop()
                //                        var from = (drag.source).visualIndex
                //                        console.log(drag.source , drag.x , drag.source.objectName)

                //                    }



                //                    onDropped: {
                //                        console.log(drag.source , drag.x , drag.source.objectName)
                //                        var dropRow = treeView.rowAt(drop.y)
                //                        var draggedIndex = drag.source.TreeView.row

                //                        treeView.model.remove(draggedIndex)
                //                        treeView.model.insert(dropRow, draggedItem)
                //                    }
                //                }
            }
        }
    }
}

