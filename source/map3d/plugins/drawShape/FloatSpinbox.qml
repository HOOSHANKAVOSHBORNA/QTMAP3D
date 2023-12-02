import QtQuick
import QtQuick.Controls.Basic 2.5
import QtQuick.Layouts
import "style"


Item{
    id: root

    function increment(){
        spinBox.increase()
    }
    function decrement(){
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

    width: 200
    height: 25

    Binding{
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


        contentItem: TextInput{
                                 id:textInput
                                 text:  textFromValue()
                                 color: Style.foregroundColor
                                 selectionColor: "#21be2b"
                                 selectedTextColor: "#ffffff"
                                 horizontalAlignment: Qt.AlignHCenter
                                 verticalAlignment: Qt.AlignVCenter
                                 readOnly: !editable
                                 validator: validator
                                 inputMethodHints: Qt.ImhFormattedNumbersOnly
            }

        down.indicator: Rectangle {
            id: downIndicator
            color: "transparent"
            anchors.leftMargin: 25
            x: spinBox.mirrored ? parent.width - width : 0
            height: parent.height
            width: 15
            Image{
                width: 15
                height: 15
                anchors.centerIn: parent
                source: "qrc:/Resources/minus.png"
            }
        }

        up.indicator: Rectangle {
            id: upIndicator
            color: "transparent"
            x: spinBox.mirrored ? 0 : parent.width - width
            height: parent.height
            width: 15
            Image{
                width: 15
                height: 15
                anchors.centerIn: parent
                source: "qrc:/Resources/add.png"
            }
        }
        background: Rectangle{
            color: backgroundColor
            radius: 15
        }

        validator: DoubleValidator {
            bottom: Math.min(spinBox.from, spinBox.to)
            top:  Math.max(spinBox.from, spinBox.to)
            decimals: root.decimals
            notation: DoubleValidator.StandardNotation
        }

        textFromValue: function() {
          return Number(spinBox.value / root.decimalFactor).toLocaleString(spinBox.locale, 'f', root.decimals)
        }

        valueFromText: function(text, locale) {
            return Math.round(Number.fromLocaleString(locale, text) * root.decimalFactor)
        }
    }
}
