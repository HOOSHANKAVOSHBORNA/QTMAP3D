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

//                flickableDirection: Qt.Horizontal

                delegate: Item {
                    id: treeDelegate
                    implicitWidth: treerootItem.width
                    implicitHeight: label.implicitHeight * 2 - 4*depth
                    anchors.margins: 10


                    readonly property real indent: 25
                    readonly property real padding: 5
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth



                    DropArea{
                        anchors.fill: parent
                        id:dropArea
                        visible: dropRole
                        onDropped: {

//                            console.log("onDropped")
//                            console.log(treeView.index(row , column))
                            if((treeView.index(row , column) !== layersModell.dragIndex) && (layersModell.dragIndex.valid)){
                                if(layersModell.dragIndex.parent === treeView.index(row , column).parent){
                                    layersModell.onMoveItem(layersModell.dragIndex, treeView.index(row , column))
                                    //print("replacing " + treeView.index(row , column) + " with: " + layersModell.dropIndex)
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
                                GradientStop{id:dropEffectColor;color: Style._darkGray ; position: 0.5 }
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
//                        console.log("drag started")
//                        console.log(treeView.index(row , column))
                        rootItem.layersModell.dragIndex = treeView.index(row, column)
//                        console.log("drag start")

                    }
                    Drag.onDragFinished: {
//                        console.log("drag finished")
                        Drag.drop();
                    }
//                    Drag.active: dragHandler.active
//                    DragHandler {
//                        id: dragHandler
//                        target: parent
//                        xAxis.enabled: false
//                        yAxis.enabled: true
//                        //                onActiveChanged:
//                        //                    console.log("drag");
//                        onGrabChanged:(trans, point)=>{
//                                          //drag
//                                          if(trans === 16){
//                                              console.log("drag")
//                                          }
//                                          if(trans === 32){
//                                              if(!dropArea.containsDrag){
//                                              }
//                                              parent.Drag.drop();
//                                              console.log("drop")
//                                          }
//                                      }
//                    }
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
                        width:  parent.width - x - 10
                        height:   parent.height-10
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
                        //                        onTapped: {

                        //                        }
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
                            color: visibleRole ?  Style._mainBlue : "red"

                        }
                        MouseArea{
                            id:hideBtn
                            //                    enabled: visibleRole
                            hoverEnabled: true
                            anchors.fill: hideContainer
//                            onEntered: eye.color = Style._mainYellow
//                            onExited: visibleRole ? eye.color = Style._mainBlue : eye.color = "red"

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
//                                enabled: row === 0 ? false:true
                                icon.source: "./Resources/48/arrow-outline-up.png"
                                icon.color: Style._persianGreen
                                onClicked: {
//                                    rootItem.layersModell.dropIndex = treeView.index(row-1 , column)
                                    rootItem.layersModell.onMoveItem(treeView.index(row , column), treeView.index(row-1 , column))
                                }

                            }
                            MenuItem {
                                text: "Shift Down"
//                                enabled: row === treeView.index(row , column).parent().rowCount() ? false:true
                                icon.source: "./Resources/48/arrow-outline-down.png"
                                icon.color: Style._persianGreen
                                onClicked: {
//                                    rootItem.layersModell.dropIndex= treeView.index(row+1 , column)
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

