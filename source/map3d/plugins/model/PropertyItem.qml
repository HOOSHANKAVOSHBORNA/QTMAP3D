import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Item {
    id: rootItem

    anchors.fill: parent

    property int lblWidth: 85
    property int valHeight: 30
    property int previousIndex: 0
    property int nextIndex: 0

    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.20)
    property var cppInterface: modelPropertyInterface

    // DEBUG
    //    Text {
    //        anchors.centerIn: parent
    //        text: 'Model Property Item'
    //    }
    //    Rectangle {
    //        anchors.fill: parent
    //        color: 'pink'
    //    }
    // ENDDEBUG
    ColumnLayout {
        width: parent.width

        // --------------------------------------------------------- Name
        RowLayout {
            id: namerowl

            Layout.fillWidth: true
            spacing: 0

            Text {
                Layout.preferredWidth: lblWidth / Style.monitorRatio
                text: "Name"
                font.pixelSize: 17 / Style.monitorRatio
                color: Style.foregroundColor
            }

            TextField {
                id: nametextf

                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true

                height: valHeight
                text: rootItem.cppInterface.name ?? "notInitial"
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
            //        visible: rootItem.model ? rootItem.cppInterface.fillColorStatus : false
            spacing: 0

            Text {
                Layout.preferredWidth: lblWidth / Style.monitorRatio
                text: "Color"
                font.pixelSize: 17 / Style.monitorRatio
                color: Style.foregroundColor
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 3 / Style.monitorRatio
                ListModel {
                    id: colorModel

                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#EF2929"
                    }
                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#FCAF3E"
                    }
                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#FCE94F"
                    }
                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#8AE234"
                    }
                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#729FCF"
                    }
                    ListElement {
                        checkIconVisible: false
                        propertyColorSelect: "#FFFFFF"
                    }
                }

                Repeater {
                    id: colorModelRepeater
                    model: colorModel

                    Button {
                        required property color propertyColorSelect
                        required property bool checkIconVisible
                        required property int index

                        checkable: true
                        implicitWidth: 26 / Style.monitorRatio
                        implicitHeight: 26 / Style.monitorRatio

                        background: Rectangle {
                            radius: width
                            color: propertyColorSelect
                        }

                        Image {
                            anchors.fill: parent
                            source: "qrc:/Resources/add-place-color-select.png"
                            visible: checkIconVisible
                        }

                        onClicked: {
                            addIconImage.visible = true
                            propertyCheckIcon.visible = false
                            colorSelectCircle.color = Style.backgroundColor

                            colorModel.setProperty(previousIndex,
                                                   "checkIconVisible", false)
                            nextIndex = index
                            if (previousIndex !== nextIndex) {
                                colorModelRepeater.itemAt(
                                            previousIndex).checked = false
                            }
                            colorModel.setProperty(nextIndex,
                                                   "checkIconVisible", checked)
                            if (checked) {
                                previousIndex = index
                                // TODO
                            } else {

                                // TODO
                            }
                        }
                    }
                }

                Rectangle {
                    id: colorSelectCircle
                    implicitWidth: 26 / Style.monitorRatio
                    implicitHeight: 26 / Style.monitorRatio
                    radius: width
                    border.width: 1 / Style.monitorRatio
                    border.color: Style.foregroundColor
                    IconImage {
                        id: addIconImage
                        anchors.centerIn: parent
                        width: 20 / Style.monitorRatio
                        height: 20 / Style.monitorRatio
                        source: "qrc:/Resources/location-add.png"
                    }

                    IconImage {
                        id: propertyCheckIcon
                        anchors.centerIn: parent
                        width: 20 / Style.monitorRatio
                        height: 20 / Style.monitorRatio
                        source: "qrc:/Resources/add-place-color-select.png"
                        visible: false
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: colorBox.visible = true
                    }
                }
            }
        }

        ColorBoxInput {
            id: colorBox

            visible: false
            onColorChosen: {
                colorSelectCircle.color = selectedColor
                rootItem.cppInterface.color = selectedColor
                addIconImage.visible = false
                propertyCheckIcon.visible = true
                colorModel.setProperty(previousIndex, "checkIconVisible", false)
            }
        }

        // --------------------------------------------------------- Location
        RowLayout {
            spacing: 0
            Layout.fillWidth: true

            Text {
                text: "Location"
                color: Style.foregroundColor
                font.pixelSize: 17 / Style.monitorRatio
                Layout.alignment: Qt.AlignTop
                Layout.preferredWidth: lblWidth / Style.monitorRatio
            }

            GroupBox {
                id: locationSec
                Layout.fillWidth: true
                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.rightMargin: 15 / Style.monitorRatio
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
                            text: "X "
                            padding: 5 / Style.monitorRatio
                            Layout.topMargin: 5 / Style.monitorRatio
                            font.pointSize: 10 / Style.monitorRatio
                            color: Style.foregroundColor
                        }

                        StepSpinBox {
                            id: xLocationValue
                            editable: true
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100 / Style.monitorRatio
                            Layout.rightMargin: 5 / Style.monitorRatio
                            Layout.topMargin: 5 / Style.monitorRatio
                            height: valHeight / Style.monitorRatio
                            decimals: 4
                            from: -180
                            to: 180
                            onValueChanged: {
                                rootItem.cppInterface.location.x = value
                            }
                        }

                        Binding {
                            target: xLocationValue
                            property: "value"
                            value: rootItem.model ? rootItem.cppInterface.location.x : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: 20 / Style.monitorRatio
                            text: "Y "
                            padding: 5 / Style.monitorRatio
                            Layout.topMargin: 5 / Style.monitorRatio
                            font.pointSize: 10 / Style.monitorRatio
                            color: Style.foregroundColor
                        }
                        StepSpinBox {
                            id: yLocationValue
                            editable: true
                            Layout.fillWidth: true
                            Layout.minimumWidth: 100 / Style.monitorRatio
                            Layout.topMargin: 5 / Style.monitorRatio
                            Layout.rightMargin: 5 / Style.monitorRatio
                            height: valHeight / Style.monitorRatio
                            decimals: 4
                            from: -180
                            to: 180
                            onValueChanged: {
                                rootItem.cppInterface.location.y = value
                            }
                        }

                        Binding {
                            target: yLocationValue
                            property: "value"
                            value: rootItem.model ? rootItem.cppInterface.location.y : 0
                            delayed: true
                        }

                        Text {
                            Layout.preferredWidth: 20 / Style.monitorRatio
                            text: "Z "
                            padding: 5 / Style.monitorRatio
                            Layout.topMargin: 5 / Style.monitorRatio
                            font.pointSize: 10
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
                            from: -180
                            to: 180
                            onValueChanged: {
                                rootItem.cppInterface.location.z = value
                            }
                        }

                        Binding {
                            target: zLocationValue
                            property: "value"
                            value: rootItem.model ? rootItem.cppInterface.location.z : 0
                            delayed: true
                        }
                    }

                    CheckBox {
                        id: relative
                        text: "Relative"
                        font.pointSize: 10 / Style.monitorRatio
                        checked: false

                        onCheckStateChanged: if (checked) {
                                                 rootItem.cppInterface.locationRelative = true
                                             } else {
                                                 rootItem.cppInterface.locationRelative = false
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
    }
}
