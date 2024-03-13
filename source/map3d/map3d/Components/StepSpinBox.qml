import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "../style"

Item {
    id: rootItem

    property alias value: floatSpinBox.value
    property alias from: floatSpinBox.from
    property alias to: floatSpinBox.to
    property alias decimals: floatSpinBox.decimals
    property alias editable: floatSpinBox.editable

    property int stepIndex: 3

    readonly property color bg30: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.30)

    RowLayout {
        anchors.fill: parent

        //        ComboBox {
        //            id: control
        //            Layout.alignment: Qt.AlignVCenter
        //            Layout.preferredHeight: 20
        //            Layout.preferredWidth: 69
        //            model: [1000, 100, 10, 1, 0.1, 0.05, 0.01]
        //            currentIndex: rootItem.stepIndex
        ////            font.pointSize: 10
        //            font.pixelSize: Style.smallFontSize
        //            displayText: model[currentIndex].toString()

        //            contentItem: Text {
        //                text: control.displayText
        //                color: Style.foregroundColor
        //                font.pixelSize:Style.smallFontSize
        //                horizontalAlignment: Qt.AlignHCenter
        //                verticalAlignment: Qt.AlignVCenter
        //            }

        //            background: Rectangle {
        //                color: bg30
        //                radius: 15
        //                IconImage {
        //                    width: 15 / Style.monitorRatio
        //                    height: 15 / Style.monitorRatio
        //                    source: "qrc:/Resources/down.png"
        //                    anchors.right: parent.right
        //                    anchors.verticalCenter: parent.verticalCenter
        //                    z: 3
        //                }
        //            }
        //            indicator: Item {}
        //        }


        ComboBox {
            id: control
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: 20 / Style.monitorRatio
            Layout.preferredWidth: 69 / Style.monitorRatio
            model: [1000, 100, 10, 1, 0.1, 0.05, 0.01]
            font.pixelSize: Style.smallFontSize
            delegate: ItemDelegate {
                width: control.width
                contentItem: Text {
                    text: control.textRole
                          ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole])
                          : modelData
                    color: Style.foregroundColor
                    //                    font.pixelSize: Style.smallFontSize
                    font: control.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: control.highlightedIndex === index
                background: Rectangle{
                    color:Style.backgroundColor
                }
            }

            indicator: IconImage {
                width: 15 / Style.monitorRatio
                height: 15 / Style.monitorRatio
                source: "qrc:/Resources/down.png"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }

            contentItem: Text {
                text: control.displayText
                color: Style.foregroundColor
                font.pixelSize:Style.smallFontSize
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }


            background: Rectangle {
                color: bg30
                radius: 15 }

            popup: Popup {
                y: control.height - 1
                width: control.width
                implicitHeight: contentItem.implicitHeight
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: control.popup.visible ? control.delegateModel : null
                    currentIndex: control.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator { }
                }

                background: Rectangle {
                    border.color: "transparent"

                    radius: 5
                }
            }
        }

        FloatSpinBox {
            id: floatSpinBox
            stepSize: control.currentValue
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
    }
}
