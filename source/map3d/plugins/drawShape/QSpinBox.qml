import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects


SpinBox {
    id: spinbox
//    anchors.centerIn: parent
    editable: true
    width: 200

    property int decimals: 4
    readonly property int decimalFactor: Math.pow(10, decimals)
    property real realValue: value / decimalFactor
    Binding{
        target: spinbox
        property: "value"
        value: spinbox.realValue * spinbox.decimalFactor
        delayed: true
    }

    stepSize: stepCombobox.currentValue * decimalFactor
    from: 0
    to: 10000 * decimalFactor
    value: 0

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / decimalFactor).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * decimalFactor
    }
    onValueChanged:{
        realValue = value / decimalFactor
    }

    contentItem: RowLayout {
        ComboBox {
            id:stepCombobox
            Layout.preferredWidth: 65
            Layout.fillHeight: true
            model: [1000, 100, 10, 1, 0.1, 0.05, 0.01 ]
            currentIndex: 3
//            onCurrentValueChanged: print(currentValue)
        }
        TextInput {
            Layout.fillHeight: true
            Layout.fillWidth: true
            z: 2
            text: spinbox.textFromValue(spinbox.value, spinbox.locale)

            font: spinbox.font
            color: "#21be2b"
            selectionColor: "#21be2b"
            selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            readOnly: !spinbox.editable
            validator: spinbox.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }

    }

    //        up.indicator: Rectangle {
    //            x: control.mirrored ? 0 : parent.width - width
    //            height: parent.height
    //            implicitWidth: 40
    //            implicitHeight: 40
    //            color: control.up.pressed ? "#e4e4e4" : "#f6f6f6"
    //            border.color: enabled ? "#21be2b" : "#bdbebf"

    //            Text {
    //                text: "+"
    //                font.pixelSize: control.font.pixelSize * 2
    //                color: "#21be2b"
    //                anchors.fill: parent
    //                fontSizeMode: Text.Fit
    //                horizontalAlignment: Text.AlignHCenter
    //                verticalAlignment: Text.AlignVCenter
    //            }
    //        }

    //        down.indicator: Rectangle {
    //            x: control.mirrored ? parent.width - width : 0
    //            height: parent.height
    //            implicitWidth: 40
    //            implicitHeight: 40
    //            color: control.down.pressed ? "#e4e4e4" : "#f6f6f6"
    //            border.color: enabled ? "#21be2b" : "#bdbebf"

    //            Text {
    //                text: "-"
    //                font.pixelSize: control.font.pixelSize * 2
    //                color: "#21be2b"
    //                anchors.fill: parent
    //                fontSizeMode: Text.Fit
    //                horizontalAlignment: Text.AlignHCenter
    //                verticalAlignment: Text.AlignVCenter
    //            }
    //        }

    //        background: Rectangle {
    //            implicitWidth: 140
    //            border.color: "#bdbebf"
    //        }
}
