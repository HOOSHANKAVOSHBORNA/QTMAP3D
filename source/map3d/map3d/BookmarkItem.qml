import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import "style"

Item {
    id: rootItem
    readonly property color backgroundColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20 / Style.monitorRatio
        //-------------------for search --------------------------------------
        Rectangle {
            Layout.fillWidth: true
            height: 30 / Style.monitorRatio
            radius: height / 2
            color: backgroundColor

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
                function sendToSearch() {
                    bookmarkproxymodel.setSearchedText(text)
                    if (text.length === 0) {
                        treeview.collapseRecursively()
                    }
                    treeview.expandRecursively()
                }
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
                onAccepted: {
                    sendToSearch()
                }
                onTextChanged: function() {
                    sendToSearch()
                }
                placeholderText: "Search ..."
                placeholderTextColor: Style.disableColor
            }
        }

        ScrollView{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10/Style.monitorRatio
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TreeView {
                id:treeview
                anchors.fill: parent
                anchors.centerIn: parent
                model: bookmarkproxymodel
                rowSpacing: 5/Style.monitorRatio

                selectionModel: bookmarkproxymodel.selectioModel()

                delegate: Item {
                    id: treeDelegate
                    readonly property real indent: 30
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    required property bool selected
                    required property bool current

                    implicitWidth: treeview.width
                    implicitHeight:  30 / Style.monitorRatio
                    // vertical line for children items
                    Rectangle{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: indent * depth
                        width: 2/Style.monitorRatio
                        height: parent.height + treeview.rowSpacing
                        visible: depth
                        color:Style.foregroundColor
                        opacity: 0.2
                    }

                    Rectangle {
                        id: itemRect
                        anchors.fill: parent
                        anchors.leftMargin: depth?(indent) * depth + 10/Style.monitorRatio: 0
                        radius: height/2
                        color: selected ? backgroundColor: mousearea.containsMouse ? backgroundColor : "transparent"
                        RowLayout{
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            spacing: 0
                            Image {
                                id: itemIcon
                                source: imageSource ?? ""
                                Layout.preferredWidth: 24
                                Layout.preferredHeight: 24
                                Layout.leftMargin: 10
                                Layout.alignment: Qt.AlignVCenter
                                visible: hasChildren
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
                            }
                            Button{
                                id:popUpBtn
                                visible: !hasChildren
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                background: Image {
                                    source: "qrc:/Resources/easy-to-find.png"
                                    width: 20
                                    height: 20
                                }
                                onClicked: {
                                    var item = bookmarkproxymodel.rowItem(treeview.index(row,column))
                                    mywnd.visible = true
                                    item.parent = testwindow
                                }
                            }
                            Button{
                                id:removeBtn
                                visible: !hasChildren
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                Layout.rightMargin: 5
                                background: Image{
                                    source:"qrc:/Resources/multiply.png"
                                    width: 20
                                    height: 20
                                }
                                onClicked: {
                                    bookmarkproxymodel.removeBookmarkItem(treeview.index(row,column))
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
                            onClicked:{
                                bookmarkproxymodel.select(treeview.index(row,column))
                                treeview.toggleExpanded(row)
                            }
                        }
                    }
                }
            }
        }
    }
}
