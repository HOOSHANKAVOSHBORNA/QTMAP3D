import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import "Components"
import "style"

Item {
    id: rootItem

    property var cppInterface
    /*: modelPropertyInterface ?? "null"*/
    property int lblWidth: 85
    property int valHeight: 30
    property int zMax: 200000

    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.20)

    anchors.fill: parent

    ScrollView {
        anchors.fill: parent
        clip: true
        contentHeight: clclcl.height
        contentWidth: parent.width

        ColumnLayout {
            id: clclcl
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
                    font.pixelSize: Style.regularFontSize
                    color: Style.foregroundColor
                }

                TextField {
                    id: nametextf

                    Layout.minimumWidth: 100 / Style.monitorRatio
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: rootItem.cppInterface ? rootItem.cppInterface.name : "notInitial"
                    font.pixelSize: Style.regularFontSize
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
                    font.pixelSize: Style.regularFontSize
                    color: Style.foregroundColor
                }

                ColorPicker {
                    id: fillColorPick

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    selectedColor: rootItem.cppInterface.color

                    onSelectedColorChanged: {
                        rootItem.cppInterface.color = selectedColor
                    }

                }
                Binding {
                    target: fillColorPick
                    property: "selectedColor"
                    value: rootItem.cppInterface ? rootItem.cppInterface.color : "blue"
                    delayed: true
                }
            }

            // --------------------------------------------------------- Location
            RowLayout {
                spacing: 0
                Layout.fillWidth: true

                Text {
                    text: "Location"
                    color: Style.foregroundColor
                    font.pixelSize: Style.regularFontSize
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: lblWidth / Style.monitorRatio
                }

                GroupBox {
                    id: locationSec
                    Layout.fillWidth: true
                    padding: 0

                    background: Rectangle {
                        color: fg20
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                    }

                    ColumnLayout {
                        anchors.fill: parent

                        GridLayout {
                            columnSpacing: 1
                            rowSpacing: 1
                            columns: 2

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "X"
                                padding: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                font.pixelSize: Style.regularFontSize
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: xLocationValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                Layout.preferredHeight: valHeight / Style.monitorRatio
                                decimals: 4
                                from: -180
                                to: 180
                                stepIndex: 4
                                onValueChanged: {
                                    rootItem.cppInterface.location.x = value
                                }
                            }

                            Binding {
                                target: xLocationValue
                                property: "value"
                                value: rootItem.cppInterface ? rootItem.cppInterface.location.x : 0
                                delayed: true
                            }

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "Y "
                                padding: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                font.pixelSize: Style.regularFontSize
                                color: Style.foregroundColor
                            }
                            StepSpinBox {
                                id: yLocationValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.preferredHeight: valHeight / Style.monitorRatio
                                decimals: 4
                                from: -180
                                to: 180
                                stepIndex: 4
                                onValueChanged: {
                                    rootItem.cppInterface.location.y = value
                                }
                            }

                            Binding {
                                target: yLocationValue
                                property: "value"
                                value: rootItem.cppInterface ? rootItem.cppInterface.location.y : 0
                                delayed: true
                            }

                            Text {
                                Layout.preferredWidth: 20 / Style.monitorRatio
                                text: "Z "
                                padding: 5 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                font.pixelSize: Style.regularFontSize
                                color: Style.foregroundColor
                            }

                            StepSpinBox {
                                id: zLocationValue

                                editable: true
                                Layout.fillWidth: true
                                Layout.minimumWidth: 100 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                height: valHeight / Style.monitorRatio
                                decimals: 4
                                from: -200
                                to: rootItem.zMax
                                stepIndex: 0
                                onValueChanged: {
                                    rootItem.cppInterface.location.z = value
                                }
                            }

                            Binding {
                                target: zLocationValue
                                property: "value"
                                value: rootItem.cppInterface ? rootItem.cppInterface.location.z : 0
                                delayed: true
                            }
                        }

                        CheckBox {
                            id: relative
                            text: "Relative"
                            font.pixelSize: Style.regularFontSize
                            checked: false

                            onCheckStateChanged: if (checked) {

                                                     // TODO
                                                 } else {

                                                     // TODO
                                                 }

                            indicator: Rectangle {
                                implicitWidth: 11 / Style.monitorRatio
                                implicitHeight: 11 / Style.monitorRatio
                                x: relative.leftPadding / Style.monitorRatio
                                y: (parent.height / 2 - height / 2) / Style.monitorRatio
                                radius: (height / 2) / Style.monitorRatio
                                border.color: relative.down ? "black" : "#313131"

                                Rectangle {
                                    width: 11 / Style.monitorRatio
                                    height: 11 / Style.monitorRatio
                                    radius: (height / 2) / Style.monitorRatio
                                    color: relative.down ? "black" : "dark green"
                                    visible: relative.checked
                                }
                            }

                            contentItem: Text {
                                text: relative.text
                                font: relative.font
                                opacity: enabled ? 1.0 : 0.3
                                color: relative.down ? "black" : Style.foregroundColor
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: relative.indicator.width + relative.spacing
                            }
                        }
                    }
                }
            }

            // --------------------------------------------------------- Speed
            RowLayout {
                id: speedSec
                spacing: 0
                Layout.fillWidth: true

//                visible: rootItem.cppInterface ? rootItem.cppInterface.isMovable : false

                Text {
                    Layout.preferredWidth: lblWidth / Style.monitorRatio
                    text: "Speed"
                    font.pixelSize: Style.regularFontSize
                    color: Style.foregroundColor
                }

                Rectangle {
                    color: fg20
                    radius: 10
                    border.color: "transparent"
                    Layout.fillWidth: true
                    Layout.preferredHeight: valHeight / Style.monitorRatio

                    StepSpinBox {
                        id: lengthValue
                        editable: true
                        anchors.leftMargin: 5 / Style.monitorRatio
                        anchors.rightMargin: 5 / Style.monitorRatio
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 30 / Style.monitorRatio
                        decimals: 2
                        from: 0
                        to: 20000000
                        onValueChanged: {
                            rootItem.cppInterface.speed = value
                        }
                    }
                    Binding {
                        target: lengthValue
                        property: "value"
                        value: rootItem.cppInterface ? rootItem.cppInterface.speed : 0
                        delayed: true
                    }
                }
            }


        }
    }
}
