import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Button {
    id: button
    property alias buttonText: buttonText.text
    property alias iconImageSource: iconImage.source
    property alias buttonColor: backgroundBtn.color
    property alias buttonTextColor: buttonText.color
    property alias iconImageVisible: iconImage.visible
    property alias buttonBorder: backgroundBtn.border
    property alias iconImageColor: iconImage.color
    hoverEnabled: true
    padding: 0

    background: Rectangle {
        id: backgroundBtn
        color: Style.foregroundColor
        radius: width / (Style.monitorRatio * 2)
    }
    contentItem: RowLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        IconImage {
            id: iconImage
            Layout.preferredWidth: 20 / Style.monitorRatio
            Layout.preferredHeight: 20 / Style.monitorRatio
            Layout.alignment: Qt.AlignVCenter
            visible: false
            color: Style.backgroundColor
        }
        Text {
            id: buttonText
            font.pixelSize: Style.regularFontSize
            Layout.alignment: Qt.AlignVCenter
            color: button.hovered ? "#01AED6" : Style.backgroundColor
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
