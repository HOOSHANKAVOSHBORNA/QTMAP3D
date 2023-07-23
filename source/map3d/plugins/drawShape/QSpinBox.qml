import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects


Item {
    id:root

    property var steps: [5000,1000,100,10,5,1,0.5,0.1,0.05,0.01,0.005,0.001]
    property int index: 5
    property double value: realValue * spinBox.decimalFactor
    property double realValue
    property int    decimals
    property double from          : -9999999999999
    property double to            : 9999999999999




    implicitHeight: 35
    implicitWidth: 200

    SpinBox {
        id: spinBox
        from: root.from * decimalFactor
        to: root.to * decimalFactor
        stepSize: decimalFactor * steps[index]
        value: root.value
        editable: true
        anchors.centerIn: parent

        property int decimals: root.decimals
//        property real realValue: value / decimalFactor
        readonly property int decimalFactor: Math.pow(10, decimals)

//        function decimalToInt(decimal) {
//            return decimal * decimalFactor
//        }

//        function intToDecimal(intNum) {
//            return intNum / decimalFactor
//        }

        validator: DoubleValidator {
            bottom: Math.min(spinBox.from, spinBox.to)
            top:  Math.max(spinBox.from, spinBox.to)
            decimals: spinBox.decimals
            notation: DoubleValidator.StandardNotation
        }
        onValueChanged: {
//            root.realValue = value / decimalFactor
//            print("real" + realValue)
//            print(root.value)
//            print(decimalToInt(value))
//            print(intToDecimal(value))
        }

        textFromValue: function(value, locale) {
            return Number(value / decimalFactor).toLocaleString(locale, 'f', spinBox.decimals)
        }

        valueFromText: function(text, locale) {
            return Math.round(Number.fromLocaleString(locale, text) * decimalFactor)
        }
    }
    Binding {
        target: root
        property: "realValue"
        value: spinBox.value / spinBox.decimalFactor
        delayed: true
    }

    Rectangle{
        anchors.right: spinBox.left
        width: spinBox.width / 3
        height: parent.height
        color: "#303030"
        Text {
            id: stepText
            text: steps[index]
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
        }
        Image {
            id: upIndicator
            anchors.right: parent.right
            anchors.top: parent.top
            source: "./resources/arrow.png"
            rotation: 90
            height: parent.height / 2
            width: parent.width / 3
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    index = index > 0 ? index -1 : index
                }
            }
        }
        Image {
            id: downIndicator
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            source: "./resources/arrow.png"
            rotation: 270
            height: parent.height / 2
            width: parent.width / 3
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    index = index < root.steps.length - 1 ? index +1 : index
                }
            }
        }
    }
}
