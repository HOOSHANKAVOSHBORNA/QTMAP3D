import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0
import "style"
Item {
    id: rootItem
    property var listModel
    readonly property color     _colorHover : Style.hoverColor
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorRec   : "#363739"
    readonly property color     sectionColor: Style.primaryColor
    readonly property real      categorySize: 40
    readonly property real      itemSize: 40

//    width: 350
    anchors.fill: parent

    Rectangle{
        id: header
        anchors.top: parent.top
    }
    Rectangle {
        id: search
        width: parent.width
        height: 40
        anchors.top: header.bottom
        color: Style._darkestGray
        clip: true
        TextField {
            function sendToSearch() {
                rootItem.listModel.setFilterString(text)
                if (text.length == 0) {
                    treeView.collapseRecursively()
                }
                treeView.expandRecursively()
            }

            background: Rectangle {
//                radius: Style.radius
                width: parent.width + 4
                height: 35
                border.color: "black"
                border.width: 1
                color: Style.backgroundColor
                anchors.centerIn: parent
                IconImage{
                    source: "./Resources/48/search.png"
                    color: "white"
                    height: parent.height * 0.6
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 5
                }
            }
            anchors.fill: parent
            color: Style.textColor
            placeholderText: "Search Toolbox"
            placeholderTextColor: Style.selectionColor
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
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
        width: parent.width
        color: Style._darkestGray
        border.color: "#24242b"
//        border.width: 10
//        anchors.topMargin: 10
        ScrollView {
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TreeView {
                id: treeView
                anchors.fill: parent
//                anchors.rightMargin: 10
//                anchors.leftMargin: 5
//                anchors.rightMargin: 5
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
//                    anchors.margins: 5
                    Rectangle{
                        id: container
                        width: parent.width  - treeDelegate.indent * ( 0.5 * treeDelegate.depth ) - treeDelegate.padding -3
                        height: parent.height
//                        border.width: 1
//                        border.color: "#202020"
                        color: "transparent"
                        clip: true
                        anchors.horizontalCenter: parent.horizontalCenter
//                        radius: Style.radius
//                        x: treeDelegate.indent * (0.68 * treeDelegate.depth ) - (treeDelegate.depth) * treeDelegate.padding

                        Rectangle {
                            id: rect
                            color: treeDelegate.hasChildren ? Style._darkGray : Style._darkestGray
                            width:  parent.width
                            height: parent.height
                            border.color: Style.backgroundColor
                            border.width: treeDelegate.hasChildren ? 2 : 0
                            radius: treeDelegate.hasChildren ? Style.radius : 0

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


                        Text {
                            id: label
                            x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
                            clip: true
                            font.pixelSize: 14
                            font.bold: treeDelegate.hasChildren
                            anchors.verticalCenter: container.verticalCenter
                            color: checkedd ? Style._darkBlue : mouseArea.containsMouse ? Style._mainBlue : "#ffffff"
                            text: display
//                            color: Style.textColor
                        }
                        Rectangle {
                            width: 5
                            height: parent.height
                            color: Style._darkGray
                            visible: !treeDelegate.hasChildren
                            x: 0
                        }

                        Rectangle {
                            width: 5
                            id: rightBar
                            height: parent.height
                            color: Style._darkGray
                            anchors.right: container.right
//                            x: container.width
                            visible: !treeDelegate.hasChildren
                        }
//                        Rectangle {
//                            width: container.width
//                            height: 5
//                            color: Style._darkestBlue
//                            x: 0
//                            visible: treeDelegate.hasChildren && treeDelegate.expanded
////                            anchors.topMargin: 3
////                            anchors.bottom: container.bottom
//                        }

                        IconImage {
                            id: img
                            source: imageSource ?? ""
                            width: 32
                            height: 32
                            x: treeDelegate.indent * (1 * treeDelegate.depth ) - (treeDelegate.depth) * treeDelegate.padding
                            anchors.verticalCenter: container.verticalCenter
                            color: checkedd ? Style._darkBlue : mouseArea.containsMouse ? _colorHover : "transparent"
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
        radius: Style.radius
        anchors.top: treeRect.bottom
        color: header.color
    }
}
