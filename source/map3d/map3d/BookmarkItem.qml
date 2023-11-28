import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import "style"
import Crystal
Item {
    id: rootItem
    property var model
    readonly property color hoverColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
    readonly property color hoverColor2: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.40)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20 / Style.monitorRatio
        //-------------------for search --------------------------------------
        Rectangle {
            Layout.fillWidth: true
            height: 30 / Style.monitorRatio
            radius: height / 2
            color: hoverColor

            IconImage {
                id: searchIcon
                source: "qrc:/Resources/search.png"
                width: 24/Style.monitorRatio
                height: 24/Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10/Style.monitorRatio
                color: Style.foregroundColor
            }

            TextField {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: searchIcon.right
                anchors.right: parent.right
                verticalAlignment: Text.AlignVCenter
                font.family: Style.fontFamily
                font.pixelSize: 17/Style.monitorRatio
                color: Style.foregroundColor
                background: Rectangle{
                    color: "transparent"
                    radius: height/2
                }
                onTextChanged: function() {
                    rootItem.model.setSearchedText(text)
                }
                placeholderText: "Search ..."
                placeholderTextColor: Style.disableColor
            }
        }

        ScrollView{
            Layout.fillWidth: true
            Layout.topMargin: 10/Style.monitorRatio
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TreeView {
                id:treeView
                anchors.fill: parent
                anchors.centerIn: parent
                model: rootItem.model
                rowSpacing: 5/Style.monitorRatio

                selectionModel: rootItem.model ? rootItem.model.selectioModel(): null

                MouseArea{
                    anchors.fill: parent
                    onClicked: {}
                }

                delegate: Item {
                    id: treeDelegate
                    readonly property real indent: 30
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    required property bool selected
                    required property bool current

                    implicitWidth: treeView.width
                    implicitHeight:  30 / Style.monitorRatio
                    // vertical line for children items
                    Rectangle{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: indent * depth
                        width: 2/Style.monitorRatio
                        height: parent.height + treeView.rowSpacing
                        visible: depth
                        color:Style.foregroundColor
                        opacity: 0.2
                    }

                    Rectangle {
                        id: itemRect
                        anchors.fill: parent
                        anchors.leftMargin: depth?(indent) * depth + 10/Style.monitorRatio: 0
                        radius: height/2
                        color: (selected && mousearea.containsMouse) ? hoverColor2 :
                                                                       (selected || mousearea.containsMouse) ? hoverColor : "transparent"
                        RowLayout{
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            spacing: 0
                            IconImage {
                                id: itemIcon
                                source: categoryIcon ?? ""
                                Layout.preferredWidth: 24
                                Layout.preferredHeight: 24
                                Layout.leftMargin: 10
                                Layout.alignment: Qt.AlignVCenter
                                visible: hasChildren
                                color: Style.foregroundColor
                            }
                            Text {
                                id: itemText
                                Layout.alignment: Qt.AlignLeft
                                Layout.leftMargin: 5
                                text: model.display
                                font.family: Style.fontFamily
                                font.pixelSize: 17/Style.monitorRatio
                                font.weight: 400
                                color: Style.foregroundColor
                                Layout.fillWidth: true
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        if(window){
                                            window.show()
                                            window.requestActivate();
                                        }
                                        rootItem.model.select(treeView.index(row,column))

                                    }
                                }
                            }
                            Button{
                                id:track
                                visible: !hasChildren
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                background: Image {
                                    source: "qrc:/Resources/track-icon.png"
                                    width: 20
                                    height: 20
                                }
                                onClicked: {
                                    rootItem.model.trackItem(treeView.index(row,column))
                                }
                            }
                            Button{
                                id:goTo
                                visible: !hasChildren
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                background: Image {
                                    source: "qrc:/Resources/easy-to-find.png"
                                    width: 20
                                    height: 20
                                }
                                onClicked: {
                                    rootItem.model.goToPosition(treeView.index(row,column))
                                }
                            }
                            Button{
                                id:removeBtn
                                visible: !hasChildren
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                Layout.rightMargin: 5
                                background: Image{
                                    source:"qrc:/Resources/garbage.png"
                                    width: 20
                                    height: 20
                                }
                                onClicked: {
                                    rootItem.model.removeBookmarkItem(treeView.index(row,column))
                                }
                            }
                            Image{
                                id:indicator
                                rotation: treeDelegate.expanded ? 180 : 0
                                visible: hasChildren
                                Layout.preferredWidth: 16
                                Layout.preferredHeight: 16
                                Layout.alignment:  Qt.AlignRight
                                Layout.rightMargin: 5
                                source: "qrc:/Resources/down.png"
                            }
                        }

                        MouseArea {
                            id: mousearea
                            anchors.fill: parent
                            hoverEnabled: true
                            propagateComposedEvents: true
                            onPressed:  (mouse)=> {
                                            rootItem.model.select(treeView.index(row,column))
                                            treeView.toggleExpanded(row)
                                            mouse.accepted = false
                                        }
                        }
                    }
                }
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
