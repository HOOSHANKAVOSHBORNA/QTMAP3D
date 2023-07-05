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


    Rectangle {
        anchors.fill: parent
        color: Style._darkestGray
        opacity: 1

        Rectangle{
            width: parent.width
            height: 80
            gradient: Gradient {
                    GradientStop { position: 0.0; color: Style._darkGray }
                    GradientStop { position: 1.0; color: Style._darkestGray }
                }

        }

        Label {
            color: "white"
            text: "Layers"
            font.family: "SourceSansPro"
            font.pointSize: 14
            anchors.horizontalCenter: parent.horizontalCenter
            y:10
        }

        Item{
            anchors.horizontalCenter: parent.horizontalCenter
            y : 45
            id:treerootItem
            width: parent.width
            height: parent.height - 40

            Rectangle {
                id: search
                width: parent.width -20
                height: 30
                y : 0
                clip: true
                //        anchors.bottom: rootItem.top
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
                        radius: Style.radius
                        implicitWidth: search.width
                        implicitHeight: 24
                        border.color: Style.borderColor
                        border.width: Style.borderwidth
                        color: Style.secondaryColor
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
                        Image {
                            id: searchIcon
                            source: "./Resources/search.png"
                            anchors.fill: parent

                        }
                    }

                    placeholderText: "Search Layers"

                    //            placeholderTextColor: "#495866"
                    placeholderTextColor: Style.selectionColor
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
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

                delegate: Item {
                    id: treeDelegate
                    implicitWidth: treerootItem.width
                    //            implicitWidth: padding + label.x + label.implicitWidth + padding
                    implicitHeight: label.implicitHeight * 2 - 4*depth

                    readonly property real indent: 20
                    readonly property real padding: 5
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth





                    Rectangle{
                        id: container
                        width:  parent.width - x - 10
                        height:   parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        color:  Style._darkestGray
                        radius:   Style.radius
                        x:  ((treeDelegate.depth ) * treeDelegate.indent)

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

                    //            Text{
                    //                id: indicator
                    //                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                    //                x:  (treeDelegate.depth * treeDelegate.indent) -3
                    //                anchors.verticalCenter: parent.verticalCenter
                    //                font.pixelSize: 20
                    //                text: treeDelegate.expanded ? "--" : "+"
                    ////                rotation: treeDelegate.expanded ? 90 : 0
                    //                padding: 3
                    //                color: Style.textColor
                    //            }

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
                        font.pixelSize: 14 - depth
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
                        //                color: Style.secondaryColor
                        //                border.color: isVisible ? Style.borderColor : "red"
                        //                radius: Style.radius
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left:  container.left
                        anchors.margins: 5
                        IconImage {
                            id:eye
                            source:  "./Resources/48/power.png"
                            width: parent.width
                            height: parent.height
                            anchors.centerIn: parent
                            color: isVisible ?  "green" : "red"
                        }
                        MouseArea{
                            id:hideBtn
                            //                    enabled: isVisible
                            hoverEnabled: true
                            anchors.fill: hideContainer
//                            onEntered: {
//                                eye.color = Style._mainYellow
//                            }
//                            onExited: {
//                                if(isVisible){
//                                    eye.color = "green"
//                                } else{
//                                    eye.color = "red"
//                                }
//                            }
                            onClicked: function() {
                                rootItem.layersModell.onItemClicked(treeView.index(row , column))

                            }
                        }
                    }
                }
            }
        }
    }
}

