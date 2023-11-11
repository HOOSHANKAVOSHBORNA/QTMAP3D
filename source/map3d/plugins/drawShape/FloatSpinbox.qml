import QtQuick
import QtQuick.Controls
import "style"

SpinBox {
    id: spinBox
    from: 0
    //value: decimalToInt(1.1)
    to: decimalToInt(100)
    stepSize: decimalFactor
    editable: true

    property int decimals: 2
    property real realValue: value / decimalFactor
    property alias downIndicator: downIndicator
    readonly property int decimalFactor: Math.pow(10, decimals)

    function decimalToInt(decimal) {
        return decimal * decimalFactor
    }

    validator: DoubleValidator {
        bottom: Math.min(spinBox.from, spinBox.to)
        top:  Math.max(spinBox.from, spinBox.to)
        decimals: spinBox.decimals
        notation: DoubleValidator.StandardNotation
    }

    textFromValue: function(value, locale) {
        return Number(value / decimalFactor).toLocaleString(locale, 'f', spinBox.decimals)
    }

    valueFromText: function(text, locale) {
        return Math.round(Number.fromLocaleString(locale, text) * decimalFactor)
    }

    up.indicator:Rectangle{
        anchors.right: spinBox.right
        anchors.verticalCenter: spinBox.verticalCenter
        width: 20
        height: 20
        color:"transparent"
        Image{
        source: "qrc:/Resources/add.png"
        anchors.fill: parent
    }
    }

    down.indicator:Rectangle{
        id:downIndicator
        anchors.left: spinBox.left
        anchors.verticalCenter: spinBox.verticalCenter
        width: 20
        height: 20
        color:"transparent"

        Image{
            source: "qrc:/Resources/minus.png"
            anchors.fill: parent
            }
    }


    background: Rectangle {
        color: foregroundColor
        radius: 15
    }
}
