import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Effects
import Crystal 1.0


Item {
    id: rootItem
    property var listModel
    readonly property color     _colorHover : "#FFCC00"
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorRec   : "#363739"
    readonly property color     sectionColor:  "#00587A"
    readonly property real      categorySize: 30
    readonly property real      itemSize: 30

    width: 300
    Rectangle{
        anchors.top: parent.top
        height: 40
        width: 300
        id: header
        radius: 10
        color: "#202020"
        Label {
            Text {
                id: headerType
                text: "Toolbox"
                color:"white"
                font.pointSize: 16
                anchors.centerIn: parent
            }
            id:headerTypeLbl
            width: parent.width/2
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

        }

        Rectangle {
            width: header.width
            height: 10
            anchors.bottom: header.bottom
            color: header.color
        }
    }
    Rectangle {
        id: treeRect
        anchors.top: header.bottom
        height: parent.height - header.height - footer.height
        width: 300
        color: _colorRec
        border.color: "#202020"
        border.width: 5
        ScrollView {
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TreeView {
                id: treeView
                anchors.fill: parent
//                anchors.rightMargin: 10
                clip: true
                model: rootItem.listModel
                signal toolboxItemClicked(string category, string name)

                delegate: Item {
                    id: treeDelegate

                    implicitWidth: 300
                    implicitHeight:  treeDelegate.hasChildren ? categorySize : itemSize

                    readonly property real indent: 30
                    readonly property real padding: 15
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    anchors.margins: 10
                    Rectangle{
                        id: container
                        width: parent.width - (treeDelegate.depth - 1)* treeDelegate.indent
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
                            border.width: treeDelegate.hasChildren ? 2 : 0
                            radius: treeDelegate.hasChildren ? 5 : 0

                        }
                        radius: height/10
                        x: padding + ((treeDelegate.depth - 1) * treeDelegate.indent)

                        Text {
                            id: label
                            x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
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
                            x: -indent
                        }

                        IconImage {
                            id: img
                            source: imageSource ?? ""
                            width: 24
                            height: 24
                            x: container.x - width + (treeDelegate.depth)*indent
                            anchors.verticalCenter: container.verticalCenter
                        }
                    }

                    HoverHandler{
                        onHoveredChanged: function() {
                            label.color = hovered ? _colorHover : "#ffffff"
                            img.color = hovered ? _colorHover : "transparent"
                        }

                    }

                    TapHandler {
                        onTapped: function() {
                            if (!treeDelegate.hasChildren){
                                treeView.model.onItemClicked(display)
                            }
                            treeView.toggleExpanded(row)
                        }

                        onPressedChanged: function() {
                            label.color = pressed ? _colorPresed : "#ffffff"
                            img.color = img.color == _colorPresed ? "transparent" : _colorPresed
                        }
                    }
                }
            }
        }
    }
    Rectangle {
        Rectangle {
            width: header.width
            height: 10
            anchors.top: footer.top
            color: header.color
        }
        id: footer
        width: header.width
        height: header.height/2
        radius: 5
        anchors.top: treeRect.bottom
        color: header.color
    }
}
