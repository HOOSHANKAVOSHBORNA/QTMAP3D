import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects
import Crystal 1.0


Rectangle {
    property var listModel
    readonly property color     _colorHover : "#FFCC00"
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorRec   : "#404040"
    readonly property color     sectionColor:  "#363739"
//    width: 300
    height: Math.max(parent.height, 430)

//    clip: true
//    visible: true

    TreeView {
        id: treeView
//        anchors.fill: parent
        clip: true
        model: parent.listModel
        height: Math.max(parent.height, 430)
        width: 300
        anchors.left: parent.left
        signal toolboxItemClicked(string category, string name)

        delegate: Item {
            id: treeDelegate

            implicitWidth: 300
            implicitHeight:  treeDelegate.hasChildren ? 30 : 35

            readonly property real indent: 30
            readonly property real padding: 15
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth

            Rectangle{
                id: container
                width: parent.width - (treeDelegate.depth - 1)* treeDelegate.indent-15
                height: parent.height
                border.width: 5
                border.color: "#202020"
                color: "transparent"
                Rectangle {
                    id: rect
                    anchors.fill: parent
                    color: treeDelegate.hasChildren ? sectionColor : _colorRec
//                    opacity: 0.8
                    border.color: "#202020"
                    border.width: treeDelegate.hasChildren ? 1 : 0
                    radius: treeDelegate.hasChildren ? 5 : 0

                }

                //                color: "transparent"
                //                                            border.color: "#ffffff"
                //                                            border.width: 1
                radius: height/10
                x: padding + ((treeDelegate.depth - 1) * treeDelegate.indent)
                //                RowLayout {
                //                anchors.fill: parent

                //                Image {
                //                    source: imageSource
                //                }

                Text {
                    id: label
                    x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
//                    width: implicitWidth
                    clip: true
                    font.pixelSize: 14
                    font.bold: treeDelegate.hasChildren
                    anchors.verticalCenter: container.verticalCenter
                    color: "#ffffff"
                    text: display
                }

                Rectangle {
                    width: indent
                    height: parent.height
                    color: "#9b9ca0"
//                    opacity: 0.8
                    x: -indent
                }

                IconImage {
                    id: img
                    source: imageSource ?? ""
                    width: 32
                    height: 32
                    x: container.x - width + (treeDelegate.depth)*indent
                    anchors.verticalCenter: container.verticalCenter
                }
                //                }
            }

            HoverHandler{
                //                                            onHoveredChanged: hovered ? container.color = "#808080" : container.color = "#454545"
                onHoveredChanged: function() {
                    label.color = hovered ? _colorHover : "#ffffff"
                    img.color = hovered ? _colorHover : "transparent"
//                    rect.color = hovered ? "#202020" : _colorRec
                }

            }

            TapHandler {
                onTapped: function() {
                    if (!treeDelegate.hasChildren){
                        treeView.model.onItemClicked(display)
                        treeView.model.expand(row);
                        treeView.model.onItemClickedTest(row);
                    }
                    treeView.toggleExpanded(row)
                }

                onPressedChanged: function() {
                    label.color = pressed ? _colorPresed : "#ffffff"
                    img.color = img.color == _colorPresed ? "transparent" : _colorPresed
                }
            }


            //        Text {
            //            id: indicator
            //            visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
            //            x: padding + (treeDelegate.depth * treeDelegate.indent)
            //            anchors.verticalCenter: label.verticalCenter
            //            text: "▸"
            //            rotation: treeDelegate.expanded ? 90 : 0
            //            padding: 4
            //            color: "#ffffff"
            //        }


        }
    }
}
