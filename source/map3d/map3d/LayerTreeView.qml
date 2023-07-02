import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0
import "style"
//#000814, #001d3d, #003566, #ffc300, #ffd60a
//#3d5a80, #98c1d9, #e0fbfc, #ee6c4d, #293241

Item{
    id:root
    width: parent.width


    readonly property real         categorySize: 30
    readonly property real         itemSize: 30
    property CLayersModel          proxyModel;



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
                if( root.proxyModel){
                root.proxyModel.setFilterString(text)
                if (text.length == 0) {
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
//            IconImage : "./Resources/search.png"
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
        anchors.topMargin: 2
        width: parent.width -20
        height: parent.height - 35
        anchors.horizontalCenter: parent.horizontalCenter
        //        anchors.fill: parent


        model: selectionModel.model
        clip: true

        selectionModel: ItemSelectionModel {
            id: selectionModel
            model: root.proxyModel ?? null

            onCurrentChanged: {

            }
            onSelectionChanged:{
            }
        }

//        signal toggleLayerEnabled(int layerIndex)
        //    signal clickedItem(QStandardItem itemIndex)


        delegate: Item {
            id: treeDelegate

            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.8

            readonly property real indent: 20
            readonly property real padding: 5
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth
//            property bool visibleLayer: true






            Rectangle{
                id: container
                width:  parent.parent.parent.width
                height:  depth < 1 ? parent.height -3 : parent.height
                anchors.verticalCenter: parent.verticalCenter
                color: depth < 1 ? Style.primaryColor : Style.secondaryColor
                radius:   depth < 1 ? Style.radius : 0
                x:  (treeDelegate.depth * treeDelegate.indent)

            }

            HoverHandler{
                //onHoveredChanged: hovered ? container.color = "#808080" : container.color = "#454545"
                onHoveredChanged: hovered ? label.color = Style.hoverColor : label.color = Style.textColor
            }

            TapHandler {
                onTapped: {

                    treeView.toggleExpanded(row)
                }
                onPressedChanged: pressed ? label.color = "orange" : !pressed ? label.color = "#ffffff" : "#ffffff"
            }


            Text {
                id: indicator
                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                x:  (treeDelegate.depth * treeDelegate.indent) +3
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 20
                text: "▸"
                rotation: treeDelegate.expanded ? 90 : 0
                padding: 5
                color: Style.textColor
            }

            Text {
                id: label
                x: padding+ 5 + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0) + hideContainer.width
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                font.pixelSize: 14
                anchors.verticalCenter: container.verticalCenter
                color: Style.textColor
                text: display
            }


            Rectangle{


                id: hideContainer
                width: container.height/1.35
                height: container.height/1.35
                color: Style.secondaryColor
                border.color: Style.borderColor
                radius: Style.radius
                anchors.verticalCenter: parent.verticalCenter
                anchors.right:  label.left
                anchors.margins: 5
                Image {
                    id:eye
                    source: isVisible ?  "./Resources/eye_open.png" : "./Resources/eye_close.png"
                    width: parent.width * 0.9
                    height: parent.height * 0.9
                    anchors.centerIn: parent
                }
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: hideContainer
                    onEntered: {
                        hideContainer.color = Style.hoverColor
                        hideContainer.border.color = "red"
                    }
                    onExited: {
                        if(isVisible){
                            hideContainer.color = Style.secondaryColor
                            hideContainer.border.color = Style.borderColor
                        } else{
                            hideContainer.color = _color11
                            hideContainer.border.color = "red"
                        }
                    }
                    onClicked: function() {
                        if(isVisible ){
                            hideContainer.border.color = "red"
                        } else{
                            hideContainer.border.color = "#111111"
                        }

//                        root.proxyModel.clickedItem(treeView.index(row , column))
                        root.proxyModel.onItemClicked(treeView.index(row , column))
//                        console.log(isVisible)
                    }
                }
            }
        }
    }
}

