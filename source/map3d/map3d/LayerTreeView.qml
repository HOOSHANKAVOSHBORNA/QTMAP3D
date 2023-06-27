import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0

//#000814, #001d3d, #003566, #ffc300, #ffd60a

Item{
    id:root
    width: parent.width
    //    property var listModel
    readonly property color        _colorHover : "#ffd60a"
    readonly property color        _colorPresed : "#ffc300"
    readonly property color        _colorRec   : "#001d3d"
    readonly property color        sectionColor:  "#003566"
    readonly property color        _darkColor: "#000814"
    readonly property real         categorySize: 30
    readonly property real         itemSize: 30

    property CLayersModel      proxyModel;
    property bool isEnabled: root.proxyModel.getLayerVisible(treeView.index(row , column))

    onIsEnabledChanged: {

    }

    Rectangle {
        id: search
        width: parent.width
        height: 30
        y : 0
        clip: true
        //        anchors.bottom: rootItem.top
        color: "#353535"
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
                radius: 2
                implicitWidth: search.width
                implicitHeight: 24
                border.color: "#333"
                border.width: 1
                color: "#808080"
            }
            anchors.fill: parent
            color: "white"
            placeholderText: "Search Layers"
            placeholderTextColor: "#495866"
            anchors.leftMargin: 5
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
        anchors.topMargin: 10
        width: parent.width
        height: parent.height - 40
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

        signal toggleLayerEnabled(int layerIndex)
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





            Rectangle{
                id: container
                width: depth < 1 ? parent.parent.parent.width - x  : parent.parent.parent.width - x - height/6
                height:  depth < 1 ? parent.height -3 : parent.height
                anchors.verticalCenter: parent.verticalCenter
                color: depth < 1 ? sectionColor : _colorRec
                //                color: "transparent"
                //border.color: "#ffffff"
                //border.width: 1
                radius:   depth < 1 ? height/6 : 0
                x: padding + (treeDelegate.depth * treeDelegate.indent)

            }

            HoverHandler{
                //onHoveredChanged: hovered ? container.color = "#808080" : container.color = "#454545"
                onHoveredChanged: hovered ? label.color = _colorHover : label.color = "#ffffff"
            }

            TapHandler {
                onTapped: {

                    //                console.log(rowAtIndex(treeView.index(row , column)))
                    //                treeView.currentIndex.parent.row
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
                color: "#ffa32b"
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
                color: "#21201f"
                border.color: "#111111"
                radius: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.right:  label.left
                anchors.margins: 5
                Image {
                    id:eye
                    source: "./Resources/eye_open.png"
                    width: parent.width * 0.9
                    height: parent.height * 0.9
                    anchors.centerIn: parent
                }
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: hideContainer
                    onEntered: {
                        hideContainer.color = sectionColor
                        hideContainer.border.color = _colorHover
                    }
                    onExited: {
                        if(isEnabled){
                            eye.source = "./Resources/eye_open.png"
                            hideContainer.color = "#21201f"
                            hideContainer.border.color = "#111111"
                        } else{
                            hideContainer.color = "#21201f"
                            hideContainer.border.color = "red"
                            eye.source = "./Resources/eye_close.png"
                        }
                    }
                    onClicked: function() {
                        if(isEnabled ){
//                            hideContainer.isEnabled = false
                            hideContainer.border.color = "red"

                        } else{
//                            hideContainer.isEnabled = true
                            hideContainer.border.color = "#111111"
                        }

                        //                    rootItem.model.toggleLayerEnabled(treeView.index(row , column));
//                                            console.log(treeView.index(row , column))
//                        rootItem.model.clickedItem( treeView.index(row , column))
                        //                    rootItem.model.clickedItem(modelIndex())
                        root.proxyModel.clickedItem(treeView.index(row , column))
                        console.log(isEnabled)
                    }
                }
            }
        }
    }
}

