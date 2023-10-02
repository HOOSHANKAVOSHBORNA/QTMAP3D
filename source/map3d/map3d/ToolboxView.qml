import QtQuick.Layouts 1.13
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0
import "style"
Item {
    id: rootItem
    property var listModel
    readonly property color bg20: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.20)
    readonly property color bg60: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.60)

    readonly property color fg75: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.75)
    readonly property color     _colorHover : Style.hoverColor
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorRec   : "#363739"
    readonly property color     sectionColor: Style.primaryColor
    readonly property real      categorySize: 30/Style.monitorRatio
    readonly property real      itemSize:30/Style.monitorRatio

    anchors.fill: parent


    anchors.topMargin: 40 / 1
    anchors.leftMargin: 20 / Style.monitorRatio
    anchors.rightMargin: 18 / Style.monitorRatio
    Rectangle{
        id: header
        anchors.top: parent.top
        color: "transparent"
    }

    Rectangle {
        id:search
        width: parent.width
        height: 30 / Style.monitorRatio

        radius: height / 2
        color: bg20
        clip: true

        TextInput {
            id: tiSearchedText
            function sendToSearch() {
                rootItem.listModel.setFilterString(text)
                if (text.length == 0) {
                    treeView.collapseRecursively()
                }
                treeView.expandRecursively()
            }
            anchors.fill: parent
            anchors.leftMargin: 15 / Style.monitorRatio
            anchors.rightMargin: 15 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
            font.family: Style.fontFamily
            font.pixelSize: 17 / Style.monitorRatio
            color: fg75
            onAccepted: {
                sendToSearch()
            }
            onTextChanged: function() {
                sendToSearch()
            }
        }

        Text {
            anchors.fill: parent
            anchors.leftMargin: 15 / Style.monitorRatio
            anchors.rightMargin: 15 / Style.monitorRatio
            verticalAlignment: tiSearchedText.verticalAlignment
            text: "Search Toolbox"
            font: tiSearchedText.font
            visible: tiSearchedText.text === ""
            color: fg75
        }
    }
    Item {
        id: treeRect
        anchors.top: search.bottom
        height: parent.height - header.height - footer.height
        width: parent.width

        ScrollView {
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TreeView {
                id: treeView
                anchors.fill: parent
                anchors.topMargin: 20/Style.monitorRatio
                clip: true
                model: rootItem.listModel
                signal toolboxItemClicked(string category, string name)

                selectionModel: ItemSelectionModel {
                    id: selectionM
                    onCurrentChanged: function(cur, pre){
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
                        color: "transparent"
                        clip: true
                        anchors.horizontalCenter: parent.horizontalCenter

                        Rectangle {
                            id: rect
                            color: treeDelegate.expanded? bg60:bg20
                            width:  parent.width
                            height: parent.height
                            radius: height / 2
                            Rectangle{
                                id:opacityRectangle
                                anchors.fill: parent
                                radius: height / 2
                                color:bg60
                                opacity: 0
                            }
                        }


                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {opacityRectangle.opacity = 1}
                            onExited:  {opacityRectangle.opacity = 0 }
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
                            color: Style.foregroundColor
                            text: display
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
                            visible: !treeDelegate.hasChildren
                        }

                        IconImage {
                            id: img
                            source: imageSource ?? ""
                            width: 24/Style.monitorRatio
                            height: 24/Style.monitorRatio
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
