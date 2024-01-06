import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "Components"
import "style"

Item {
    id: rootItem

    property var cppInterface

    property int lblWidth: 85 / Style.monitorRatio
    property int valHeight: 30 / Style.monitorRatio
    property int zMax: 200000

    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.20)

    ScrollView {
        anchors.fill: parent
        clip: true
        contentHeight: contentsCl.height
        contentWidth: parent.width

        ColumnLayout {
            id: contentsCl

            width: parent.width

            // --------------------------------------------------------- Name
            RowLayout {
                id: namerowl

                Layout.preferredHeight: valHeight
                Layout.fillWidth: true
                spacing: 0

                Text {
                    Layout.fillHeight: true
                    Layout.preferredWidth: lblWidth / Style.monitorRatio
                    text: "Name"
                    font.pixelSize: 17 / Style.monitorRatio
                    color: Style.foregroundColor
                }

                TextField {
                    id: nametextf

                    Layout.minimumWidth: 100 / Style.monitorRatio
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: rootItem.cppInterface ? rootItem.cppInterface.name : "notInitial"
                    font.pointSize: 10 / Style.monitorRatio
                    color: "black"

                    background: Rectangle {
                        color: fg20
                        radius: height / 2
                    }

                    onAccepted: {
                        rootItem.cppInterface.name = nametextf.text
                    }
                }
            }

            // --------------------------------------------------------- Color
            RowLayout {
                id: fillcolorSec

                Layout.fillWidth: true
                Layout.preferredHeight: valHeight
                spacing: 0

                Text {
                    Layout.preferredWidth: lblWidth / Style.monitorRatio
                    Layout.fillHeight: true
                    text: "Color"
                    font.pixelSize: 17 / Style.monitorRatio
                    color: Style.foregroundColor
                }

                ColorPicker {
                    id: fillColorPick

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //                        selectedColor: '#099999'
                    onSelectedColorChanged: {
                        rootItem.cppInterface.color = selectedColor
                    }
                }
            }

            // --------------------------------------------------------- Opacity
            OpacitySlider {
                Layout.fillWidth: true
                Layout.preferredHeight: 20
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                color: 'red'
            }
        }
    }
}
