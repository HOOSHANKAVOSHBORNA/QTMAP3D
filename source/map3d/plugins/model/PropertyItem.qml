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

                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true
                Layout.rightMargin: 15 / Style.monitorRatio
                Layout.leftMargin: 15 / Style.monitorRatio

                height: valHeight
                text: modelPropertyInterface.name ?? "notInitial"
                font.pointSize: 10 / Style.monitorRatio
                color: "black"

                background: Rectangle {
                    color: fg20
                    radius: height / 2
                }

                onAccepted: {
                    modelPropertyInterface.name = nametextf.text
                }
            }
        }

        // --------------------------------------------------------- Color
        RowLayout {
            id: fillcolorSec
            Layout.fillWidth: true
            //        visible: rootItem.model ? rootItem.model.fillColorStatus : false
            spacing: 0
            Text {
                Layout.preferredWidth: lblWidth / Style.monitorRatio
                text: "Color"
                font.pixelSize: 17 / Style.monitorRatio
                color: Style.foregroundColor
            }
            RowLayout {
                Layout.rightMargin: 15 / Style.monitorRatio
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
                rootItem.model.fillColor = selectedColor
                addIconImage.visible = false
                propertyCheckIcon.visible = true
                colorModel.setProperty(previousIndex, "checkIconVisible", false)
            }
        }

        ColorBoxInput {
            id: colorBoxStroke

            visible: false
            onColorChosen: {
                strokeColorCircle.color = selectedColor
                rootItem.model.strokeColor = selectedColor
                strokeColorAddIcon.visible = false
                strokeCheckIcon.visible = true
                strokeColorModel.setProperty(strokePreviousIndex,
                                             "checkIconVisible", false)
            }
        }

        ColorBoxInput {
            id: colorBoxPointColor

            visible: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            onColorChosen: {
                pointColorCircle.color = selectedColor
                rootItem.model.pointsColor = selectedColor
                pointColorAddIcon.visible = false
                pointColorCheckIcon.visible = true
                pointColorModel.setProperty(pointColorPreviousIndex,
                                            "checkIconVisible", false)
                console.log(rootItem.model.pointsColor)
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: 'Model Property Item'
    }
}
