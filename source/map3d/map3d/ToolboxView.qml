import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects


TreeView{
    id: treeView
    anchors.fill: parent
//    model: layersModel
    signal toolboxItemClicked(string category, string name)
    delegate: Item {
        id: treeDelegate

        implicitWidth: padding + label.x + label.implicitWidth + padding
        implicitHeight: label.implicitHeight * 1.5

        readonly property real indent: 20
        readonly property real padding: 5
        required property TreeView treeView
        required property bool isTreeNode
        required property bool expanded
        required property int hasChildren
        required property int depth

        Rectangle{
            id: container
            width: parent.width
            height: parent.height
//            color: "#454545"
                color: "transparent"
//                                            border.color: "#ffffff"
//                                            border.width: 1
            radius: height/10
            x: padding + (treeDelegate.depth * treeDelegate.indent)

        }

        HoverHandler{
//                                            onHoveredChanged: hovered ? container.color = "#808080" : container.color = "#454545"
            onHoveredChanged: hovered ? label.color = "#999999" : label.color = "#ffffff"
        }

        TapHandler {
            onTapped: function() {
                if (!treeDelegate.hasChildren){
                    treeView.model.onItemClicked(display)
                }
                treeView.toggleExpanded(row)
            }

            onPressedChanged: pressed ? label.color = "orange" : !pressed ? label.color = "#ffffff" : "#ffffff"

        }


        Text {
            id: indicator
            visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
            x: padding + (treeDelegate.depth * treeDelegate.indent)
            anchors.verticalCenter: label.verticalCenter
            text: "▸"
            rotation: treeDelegate.expanded ? 90 : 0
            padding: 4
            color: "#ffffff"
        }

        Text {
            id: label
            x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
            width: treeDelegate.width - treeDelegate.padding - x
            clip: true
            font.pixelSize: 14
            anchors.verticalCenter: container.verticalCenter
            color: "#ffffff"
            text: display
        }
    }
}
