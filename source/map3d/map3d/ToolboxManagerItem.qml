import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects

import "style"

Item {
    id: rootItem

    property var toolboxCpp
    property var toolboxModel

    readonly property color backgroundColor: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            height: 30 / Style.monitorRatio
            radius: height / 2
            color: backgroundColor

            IconImage {
                id: searchIcon
                source: "qrc:/Resources/search.png"
                width: 24 / Style.monitorRatio
                height: 24 / Style.monitorRatio
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10 / Style.monitorRatio
                color: Style.foregroundColor
            }

            TextField {
                function sendToSearch() {
                    rootItem.toolboxCpp.setFilterString(text)
                    if (text.length === 0) {
                        treeView.collapseRecursively()
                    }
                    treeView.expandRecursively()
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: searchIcon.right
                anchors.right: parent.right
                verticalAlignment: Text.AlignVCenter
                font.family: Style.fontFamily
                font.pixelSize: Style.regularFontSize
                color: Style.foregroundColor
                background: Rectangle {
                    color: "transparent"
                    radius: height / 2
                }

                onAccepted: {
                    sendToSearch()
                }
                onTextChanged: function () {
                    sendToSearch()
                }
                placeholderText: "Search ..."
                placeholderTextColor: Style.disableColor
            }
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Vertical

            ScrollView {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                //                Layout.topMargin: 10 / Style.monitorRatio
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                TreeView {
                    id: treeView
                    anchors.fill: parent
                    clip: true
                    model: rootItem.toolboxModel
                    rowSpacing: 5 / Style.monitorRatio

                    selectionModel: ItemSelectionModel {
                        id: selectionM
                        onCurrentChanged: function (cur, pre) {
                            select(cur, ItemSelectionModel.Select)
                            treeView.model.onItemClicked(cur)
                        }
                        onSelectionChanged: function (sel, des) {
                            reset()
                        }
                    }

                    delegate: Item {

                        readonly property real indent: 30
                        required property TreeView treeView
                        required property bool isTreeNode
                        required property bool expanded
                        required property int hasChildren
                        required property int depth
                        required property bool selected
                        required property bool current

                        implicitWidth: treeView ? treeView.width ?? 0 : 0
                        implicitHeight: 30 / Style.monitorRatio

                        //vertical line for item children
                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: indent * depth
                            width: 2 / Style.monitorRatio
                            height: parent.height + (treeView ? treeView.rowSpacing
                                                                ?? 0 : 0)
                            visible: depth
                            color: Style.foregroundColor
                            opacity: 0.2
                        }

                        Rectangle {
                            id: itemRect
                            anchors.fill: parent
                            anchors.leftMargin: depth ? (indent) * depth + 10
                                                        / Style.monitorRatio : 0
                            color: (expanded
                                    || checkedd) ? backgroundColor : "transparent"
                            radius: height / 2

                            IconImage {
                                id: itemIcon
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 15 / Style.monitorRatio
                                source: imageSource ?? ""
                                width: 24 / Style.monitorRatio
                                height: 24 / Style.monitorRatio
                                color: Style.foregroundColor
                            }

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: itemIcon.right
                                anchors.leftMargin: 10 / Style.monitorRatio
                                clip: true
                                font.pixelSize: Style.regularFontSize
                                font.weight: Font.Medium
                                color: Style.foregroundColor
                                text: display
                            }

                            IconImage {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10 / Style.monitorRatio
                                source: "qrc:/Resources/down.png"
                                width: 16 / Style.monitorRatio
                                height: 16 / Style.monitorRatio
                                visible: hasChildren
                                rotation: expanded ? 180 : 0
                                color: Style.foregroundColor
                            }
                        }

                        Rectangle {
                            id: selectRect
                            anchors.fill: parent
                            anchors.leftMargin: depth ? (indent) * depth + 10
                                                        / Style.monitorRatio : 0
                            radius: height / 2
                            visible: false
                            z: -1
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                selectRect.color = backgroundColor
                                selectRect.visible = true
                            }
                            onExited: {
                                selectRect.visible = false
                            }
                            onPressed: function (mouse) {
                                mouse.accepted = false
                            }
                        }

                        TapHandler {
                            onTapped: function () {
                                treeView.toggleExpanded(row)
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: propertySection

                visible: propertyContainer.children.length

                //                SplitView.preferredHeight: 300 / Style.monitorRatio
                SplitView.preferredHeight: 450 / Style.monitorRatio
                SplitView.fillWidth: true
                color: 'transparent'

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Text {
                        color: Style.foregroundColor
                        Layout.fillWidth: true
                        Layout.preferredHeight: contentHeight
                        text: toolboxCpp ? toolboxCpp.propertyItemTitle : ""
                        font.family: Style.fontFamily
                        font.pixelSize: Style.titleFontSize

                        Layout.bottomMargin: 1
                        //                    Layout.topMargin: 25 / Style.monitorRatio
                        Layout.topMargin: 10 / Style.monitorRatio
                        Layout.leftMargin: 3 / Style.monitorRatio
                    }

                    Rectangle {
                        color: Style.foregroundColor
                        Layout.fillWidth: true
                        Layout.preferredHeight: 2 / Style.monitorRatio
                        Layout.bottomMargin: 20 / Style.monitorRatio

                        radius: 1
                    }

                    Rectangle {
                        color: 'transparent'

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Item {
                            id: propertyContainer

                            clip: true

                            anchors.fill: parent
                            data: toolboxCpp? toolboxCpp.propertyItem : null
                        }
                    }
                }
            }
        }
    }
}
