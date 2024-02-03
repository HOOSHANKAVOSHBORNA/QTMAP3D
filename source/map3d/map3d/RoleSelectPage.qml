import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

ColumnLayout {
    anchors.fill: parent
    anchors.leftMargin: 50 / Style.monitorRatio
    anchors.rightMargin: 50 / Style.monitorRatio
    spacing: 0

    RowLayout {
        spacing: 0
        Layout.topMargin: 5 / Style.monitorRatio
        IconImage {
            source: "qrc:/Resources/user.png"
            Layout.preferredHeight: 25 / Style.monitorRatio
            Layout.preferredWidth: 25 / Style.monitorRatio
        }

        Text {
            text: usernameText.text
            font.pixelSize: 23 / Style.monitorRatio
            color: Style.foregroundColor
        }
    }

    Text {
        text: "Select role"
        Layout.topMargin: 30 / Style.monitorRatio
        font.pixelSize: 22 / Style.monitorRatio
        font.bold: true
        color: Style.foregroundColor
    }

    RadioButton {
        id: administrator
        text: "Administrator"
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: administrator.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }

    RadioButton {
        id: admin
        text: "Admin"
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: admin.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }

    RadioButton {
        id: reviewer
        text: "Reviewer"
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: reviewer.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }
}
