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

    readonly property color fg20: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
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
        color: fg20
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
            anchors.leftMargin: 40 / Style.monitorRatio
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
        IconImage {
            id: iconImage
            source: "qrc:/Resources/search.png"
            width: 24/Style.monitorRatio
            height: 24/Style.monitorRatio
x:10/Style.monitorRatio
            anchors.verticalCenter: parent.verticalCenter
            color: Style.foregroundColor
        }
        Text {
            id:textOfSearch
            anchors.fill: parent
            anchors.leftMargin: 40 / Style.monitorRatio
            anchors.rightMargin: 15 / Style.monitorRatio
            verticalAlignment: tiSearchedText.verticalAlignment
            text: "Search "
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
                        id:backgroundrec
//                        anchors.top: container.top
//                        anchors.left: container.left
                        width: rect.width
                      height: 20
//                        height: treeDelegate.hasChildren ? (rootItem.listModel ? (rootItem.listModel.childCount1(treeView.index(row,column)) +1) * rect.height: 0) :0
                        radius: 12
                        visible: treeDelegate.expanded
                        color:"red"
//                            opacity: 0.1
                    }
                    Rectangle{
                        id: container
                        width: parent.width
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
//                            MultiEffect {
//                                source: opacityRectangle
//                                enabled: true
//                                anchors.fill: opacityRectangle
//                                shadowColor: "black"
//                                shadowEnabled: true
//                                shadowBlur: 0.1
//                                shadowHorizontalOffset: 1.5
//                                shadowVerticalOffset:0.5
//                                shadowOpacity:0.05
//                                shadowScale: 0.98
//                            }
                        }


                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {opacityRectangle.opacity = 1
                            }
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
                            font.pixelSize: 17/Style.monitorRatio
                            anchors.verticalCenter: container.verticalCenter
                            font.weight: Font.Medium
                            color: Style.foregroundColor
                            text: display
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
                            source: "qrc:/Resources/down.png"
                            width: 16/Style.monitorRatio
                            height: 16/Style.monitorRatio
opacity: 0.75
                            anchors.rightMargin: 10/Style.monitorRatio
                            anchors.right: parent.right
                            visible: treeDelegate.hasChildren
                            rotation: treeDelegate.expanded ? 180: 0
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
