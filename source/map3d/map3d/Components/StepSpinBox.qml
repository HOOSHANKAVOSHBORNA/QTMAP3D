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

        ComboBox {
            id: stepCombobox
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: 20
            Layout.preferredWidth: 69
            model: [1000, 100, 10, 1, 0.1, 0.05, 0.01]
            currentIndex: rootItem.stepIndex
//            font.pointSize: 10
            font.pixelSize: 15 / Style.monitorRatio
            displayText: model[currentIndex].toString()

            contentItem: Text {
                text: stepCombobox.displayText
                color: Style.foregroundColor
                font.pixelSize: 15 / Style.monitorRatio
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            background: Rectangle {
                color: bg30
                radius: 15
                IconImage {
                    width: 15 / Style.monitorRatio
                    height: 15 / Style.monitorRatio
                    source: "qrc:/Resources/down.png"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    z: 3
                }
            }
            indicator: Item {}
        }

        FloatSpinBox {
            id: floatSpinBox
            stepSize: stepCombobox.currentValue
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
    }
}
