import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0

//#000814, #001d3d, #003566, #ffc300, #ffd60a
//#3d5a80, #98c1d9, #e0fbfc, #ee6c4d, #293241

Item{
    id:root
    width: parent.width
    //    property var listModel
    readonly property color        _colorHover : "#e0fbfc"
    readonly property color        _colorPresed : "#ee6c4d"
    readonly property color        _colorRec   : "#98c1d9"
    readonly property color        sectionColor:  "#3d5a80"
    readonly property color        _darkColor: "#293241"
    readonly property real         categorySize: 30
    readonly property real         itemSize: 30
    property CLayersModel      proxyModel;



    Rectangle {
        id: search
        width: parent.width
        height: 30
        y : 0
        clip: true
        //        anchors.bottom: rootItem.top
        color: "transparent"
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        TextField {
            function sendToSearch() {
                root.proxyModel.setFilterString(text)
                if (text.length == 0) {
                    treeView.collapseRecursively()
                }
                treeView.expandRecursively()
            }


            background: Rectangle {
                radius: height/2
                implicitWidth: search.width
                implicitHeight: 24
                border.color: sectionColor
                border.width: 2
                color: _darkColor
            }
            anchors.fill: parent
            color: "white"
//            IconImage : "./Resources/search.png"
            IconImage{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.6
                width: height
                anchors.right: parent.right
                anchors.rightMargin: 10
                Image {
                    id: searchIcon
                    source: "./Resources/search.png"
                    anchors.fill: parent
                }
            }

            placeholderText: "Search Layers"

//            placeholderTextColor: "#495866"
            placeholderTextColor: _colorRec
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
        width: parent.width - 24
        height: parent.height - 35
        anchors.horizontalCenter: parent.horizontalCenter
        //        anchors.fill: parent


        model: selectionModel.model
        clip: true

        selectionModel: ItemSelectionModel {
            id: selectionModel
            model: root.proxyModel

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
                width: depth < 1 ? parent.parent.parent.width - x  : parent.parent.parent.width - x - height/6
                height:  depth < 1 ? parent.height -3 : parent.height
                anchors.verticalCenter: parent.verticalCenter
                color: depth < 1 ? sectionColor : _darkColor
                radius:   depth < 1 ? height/6 : 0
                x:  (treeDelegate.depth * treeDelegate.indent)

            }

            HoverHandler{
                //onHoveredChanged: hovered ? container.color = "#808080" : container.color = "#454545"
                onHoveredChanged: hovered ? label.color = _colorHover : label.color = "#ffffff"
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
                color: _colorRec
            }

            Text {
                id: label
                x: padding+ 5 + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0) + hideContainer.width
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                font.pixelSize: 14
                anchors.verticalCenter: container.verticalCenter
                color: "#ffffff"
                text: display
            }


            Rectangle{


                id: hideContainer
                width: container.height/1.35
                height: container.height/1.35
                color: _darkColor
                border.color: _colorRec
                radius: 5
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
                        hideContainer.color = _colorPresed
                        hideContainer.border.color = "red"
                    }
                    onExited: {
                        if(isVisible){
                            hideContainer.color = _darkColor
                            hideContainer.border.color = _colorRec
                        } else{
                            hideContainer.color = _darkColor
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
                        console.log(isVisible)
                    }
                }
            }
        }
    }
}

