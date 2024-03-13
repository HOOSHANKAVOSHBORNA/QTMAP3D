import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"
import "Components"

Window {
    id: window
    readonly property color verticalBarColor: Qt.rgba(Style.foregroundColor.r,
                                                      Style.foregroundColor.g,
                                                      Style.foregroundColor.b,
                                                      0.10)
    readonly property color parentColor: Qt.rgba(Style.foregroundColor.r,
                                                 Style.foregroundColor.g,
                                                 Style.foregroundColor.b, 0.20)
    readonly property color bottomImageColor: Qt.rgba(Style.backgroundColor.r,
                                                      Style.backgroundColor.g,
                                                      Style.backgroundColor.b,
                                                      0.75)
    flags: Qt.Window | Qt.FramelessWindowHint
    width: 310 / Style.monitorRatio
    height: 612 / Style.monitorRatio
    color: "transparent"

    property var nodeInfoModel
    property string iconUrl: ""
    property string imageUrl: ""

    signal track
    signal goToPosition

    Rectangle {
        color: Style.backgroundColor
        anchors.fill: parent
        radius: 20 / Style.monitorRatio
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        Item {
            id: windowFrame
            implicitHeight: 30 / Style.monitorRatio
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 15 / Style.monitorRatio
                anchors.rightMargin: 15 / Style.monitorRatio
                anchors.topMargin: 5 / Style.monitorRatio
                anchors.bottomMargin: 5 / Style.monitorRatio
                spacing: 5 / Style.monitorRatio
                Layout.preferredHeight: 30 / Style.monitorRatio
                Layout.fillWidth: true

                IconImage {
                    Layout.preferredHeight: 22 / Style.monitorRatio
                    Layout.preferredWidth: 22 / Style.monitorRatio
                    source: iconUrl
                    // color: Style.foregroundColor
                }
                Text {
                    text: title
                    color: Style.foregroundColor
                    font.pixelSize: Style.titleFontSize
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                }

                Button {
                    Layout.preferredWidth: 22 / Style.monitorRatio
                    Layout.preferredHeight: 22 / Style.monitorRatio
                    background: IconImage {
                        width: 22 / Style.monitorRatio
                        height: 22 / Style.monitorRatio
                        source: "qrc:/Resources/multiply.png"
                    }
                    onClicked: {
                        window.close()
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onPositionChanged: startSystemMove()
                z: -1
            }
        }

        Image {
            id: image
            Layout.preferredHeight: 175 / Style.monitorRatio
            Layout.fillWidth: true
            source: imageUrl

            Rectangle {
                color: bottomImageColor
                anchors.bottom: image.bottom
                width: image.width
                height: 30 / Style.monitorRatio

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    CustomButton {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        buttonText: "Track"
                        buttonColor: "transparent"
                        buttonTextColor: Style.foregroundColor
                        iconImageColor: Style.foregroundColor
                        iconImageVisible: true
                        iconImageSource: "qrc:/Resources/track-icon.png"
                        onClicked: {
                            track()
                        }
                    }

                    Rectangle {
                        id: verticalbar
                        color: verticalBarColor
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        Layout.preferredWidth: 2 / Style.monitorRatio
                        Layout.alignment: Qt.AlignCenter
                    }
                    CustomButton {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        buttonText: "Go to"
                        buttonColor: "transparent"
                        buttonTextColor: Style.foregroundColor
                        iconImageColor: Style.foregroundColor
                        iconImageVisible: true
                        iconImageSource: "qrc:/Resources/easy-to-find.png"
                        onClicked: {
                            goToPosition()
                        }
                    }
                }
            }
        }
        ScrollView {
            id: treeViewItem
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10 / Style.monitorRatio
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TreeView {
                id: treeView
                anchors.fill: parent
                anchors.centerIn: parent
                model: nodeInfoModel
                rowSpacing: 10 / Style.monitorRatio

                delegate: Item {
                    id: treeDelegate
                    required property TreeView treeView
                    required property bool isTreeNode
                    required property bool expanded
                    required property int hasChildren
                    required property int depth
                    required property bool selected
                    required property bool current

                    implicitWidth: treeView.width
                    implicitHeight: 20 / Style.monitorRatio

                    Rectangle {
                        color: treeDelegate.hasChildren ? parentColor : "transparent"
                        radius: 15 / Style.monitorRatio
                        width: 300 / Style.monitorRatio
                        height: 25 / Style.monitorRatio
                        anchors.horizontalCenter: parent.horizontalCenter
                        RowLayout {
                            anchors.fill: parent
                            spacing: 0

                            IconImage {
                                Layout.preferredHeight: 19 / Style.monitorRatio
                                Layout.preferredWidth: 19 / Style.monitorRatio
                                source: iconImageSource ?? ""
                                Layout.alignment: Qt.AlignVCenter
                                Layout.leftMargin: 10
                                visible: treeDelegate.hasChildren
                            }

                            Text {
                                text: display ?? ""
                                font.pixelSize: Style.regularFontSize
                                color: Style.foregroundColor
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                            IconImage {
                                id: indicator
                                Layout.preferredHeight: 15 / Style.monitorRatio
                                Layout.preferredWidth: 15 / Style.monitorRatio
                                source: "qrc:/Resources/down.png"
                                Layout.alignment: Qt.AlignVCenter
                                Layout.rightMargin: 10 / Style.monitorRatio
                                rotation: treeDelegate.expanded ? 180 : 0
                                visible: treeDelegate.hasChildren
                            }
                        }

                        TapHandler {
                            onTapped: {
                                treeView.toggleExpanded(row)
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 0
                        Rectangle {
                            color: "transparent"
                            Layout.preferredHeight: 10 / Style.monitorRatio
                            Layout.preferredWidth: 160 / Style.monitorRatio
                            Text {
                                text: nameText ?? ""
                                font.pixelSize: Style.regularFontSize
                                font.family: Style.fontFamily
                                color: Style.foregroundColor
                                anchors.left: parent.left
                                anchors.leftMargin: 15 / Style.monitorRatio
                            }
                        }
                        Rectangle {
                            color: "transparent"
                            Layout.preferredHeight: 10 / Style.monitorRatio
                            Layout.preferredWidth: 160 / Style.monitorRatio
                            Text {
                                text: valueText ?? ""
                                font.pixelSize: Style.regularFontSize
                                font.family: Style.fontFamily
                                color: Style.foregroundColor

                                anchors.left: parent.left
                                anchors.leftMargin: 15 / Style.monitorRatio
                            }
                        }
                    }
                }
            }
        }
    }
}
