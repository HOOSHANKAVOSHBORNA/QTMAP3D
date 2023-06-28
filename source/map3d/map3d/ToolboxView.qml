import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls
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
        id: search
        width: header.width
        height: 30
        anchors.top: header.bottom
        color: "#202020"
        TextField {
            function sendToSearch() {
                rootItem.listModel.setFilterString(text)
                if (text.length == 0) {
                    treeView.collapseRecursively()
                }
                treeView.expandRecursively()
            }

            background: Rectangle {
                radius: 2
                implicitWidth: 100
                implicitHeight: 24
                border.color: "#333"
                border.width: 1
                color: "#454545"
            }
            anchors.fill: parent
            color: "white"
            placeholderText: "search toolbox"
            placeholderTextColor: "#656565"
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            onAccepted: {
                sendToSearch()
            }
            onTextChanged: function() {
                sendToSearch()
            }

        }
    }

    Rectangle {
        id: treeRect
        anchors.top: search.bottom
        height: parent.height - header.height - footer.height
        width: 300
        color: _colorRec
        border.color: "#202020"
        border.width: 10
        ScrollView {
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TreeView {
                id: treeView
                anchors.fill: parent
//                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.topMargin: 3
                clip: true
                model: rootItem.listModel
                signal toolboxItemClicked(string category, string name)

                selectionModel: ItemSelectionModel {
                    id: selectionM
                    onCurrentChanged: function(cur, pre){
//                        print("previous: ", treeView.model.data(pre))
//                        print("current: ", treeView.model.data(cur))
                        select(cur, ItemSelectionModel.Select)
                        treeView.model.onItemClicked(cur)
                    }
                    onSelectionChanged: function(sel, des){
                        reset()
//                        img2.rotation : treeDelegate.hasChildren ? -90 : 180
                    }
                }
                delegate: Item {
                    id: treeDelegate

                    implicitWidth: treeRect.width
                    implicitHeight:  treeDelegate.hasChildren ? categorySize : itemSize
                    readonly property real indent: 30
                    readonly property real padding: 15
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    required property bool selected
                    required property bool current
                    anchors.margins: 10
                    Rectangle{
                        id: container
                        width: parent.width - (treeDelegate.depth - 1)* treeDelegate.indent - 30
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
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onPressed: function(mouse) {
                                mouse.accepted = false

                            }
                            propagateComposedEvents: true

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
                            color: checkedd ? _colorPresed : mouseArea.containsMouse ? _colorHover : "#ffffff"
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
                            color: checkedd ? _colorPresed : mouseArea.containsMouse ? _colorHover : "transparent"
                        }
                        IconImage {
                            id: img2
                            source: "qrc:/Resources/arrow.png"
                            width: 18
                            height: 18

                            anchors.rightMargin: 5
//                            anchors.top :parent.top
                            anchors.right: parent.right
                            visible: treeDelegate.hasChildren
                            rotation: treeDelegate.expanded ? -90 : 180
                            anchors.verticalCenter: parent.verticalCenter

                        }
                    }

                    TapHandler {
                        onTapped: function() {
                            treeView.toggleExpanded(row)
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
