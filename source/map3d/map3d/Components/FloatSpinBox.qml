import QtQuick
import QtQuick.Controls.Basic 2.5
import QtQuick.Layouts

import "../style"

Item {
    id: root

    function increment() {
        spinBox.increase()
    }
    function decrement() {
        spinBox.decrease()
    }

    property int decimals: 2
    readonly property int decimalFactor: Math.pow(10, decimals)
    property real value: 0.0
    property real from: 0.0
    property real to: 100.0
    property real stepSize: 0.1

    property alias spinBox: spinBox
    property alias locale: spinBox.locale
    property alias editable: spinBox.editable
    property alias down: spinBox.down
    property alias up: spinBox.up
    property alias validator: spinBox.validator
    property alias textFromValue: spinBox.textFromValue
    property alias valueFromText: spinBox.valueFromText
    property alias contentItem: spinBox.contentItem
    property alias background: spinBox.background
    property alias leftInset: spinBox.leftInset

    readonly property color bg30: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.30)

    width: 97 / Style.monitorRatio
    height: 20 / Style.monitorRatio

    Binding {
        target: root
        property: "value"
        value: spinBox.value / decimalFactor
        delayed: true
    }

    SpinBox {
        id: spinBox
        anchors.fill: parent
        value: root.value * decimalFactor
        from: root.from * decimalFactor
        to: root.to * decimalFactor
        stepSize: root.stepSize * decimalFactor
        contentItem: TextInput {
            id: textInput
            maximumLength: 8
            text: textFromValue()
            font.pixelSize: Style.smallFontSize
            color: Style.foregroundColor
            selectionColor: Style.backgroundColor
            selectedTextColor: Style.foregroundColor
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            readOnly: !editable
            validator: validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }

        down.indicator: Rectangle {
            id: downIndicator
            color: "transparent"
            anchors.leftMargin: 25 / Style.monitorRatio
            x: spinBox.mirrored ? parent.width - width : 0
            height: parent.height
            width: 15 / Style.monitorRatio

            IconButton{
                id:iconMinus
                width: 15 / Style.monitorRatio
                height: 15 / Style.monitorRatio
                anchors.centerIn: parent
                iconImageSource:"qrc:/Resources/minus.png"
                backgroundColor: "transparent"
            }
        }

        up.indicator: Rectangle {
            id: upIndicator
            color: "transparent"
            x: spinBox.mirrored ? 0 : parent.width - width
            height: parent.height
            width: 15 / Style.monitorRatio

            IconButton{
                id:iconPlus
                width: 15 / Style.monitorRatio
                height: 15 / Style.monitorRatio
                anchors.centerIn: parent
                iconImageSource:"qrc:/Resources/add.png"
                backgroundColor: "transparent"
            }
        }

        background: Rectangle {
            color: bg30
            radius: 15
        }

        validator: DoubleValidator {
            bottom: Math.min(spinBox.from, spinBox.to)
            top: Math.max(spinBox.from, spinBox.to)
            decimals: root.decimals
            notation: DoubleValidator.StandardNotation
        }

        textFromValue: function () {
            return Number(spinBox.value / root.decimalFactor).toLocaleString(
                        spinBox.locale, 'f', root.decimals)
        }

        valueFromText: function (text, locale) {
            return Math.round(Number.fromLocaleString(
                                  locale, text) * root.decimalFactor)
        }
    }
}
