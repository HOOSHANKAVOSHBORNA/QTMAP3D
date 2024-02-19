import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

ColumnLayout {
    property var roleSelectionModel: undefined
    property alias connectionStatus: connectionStatus
    property alias signInBtn: signInBtn
    property alias backBtn: backBtn
    property alias usernameTxt: usernameTxt.text
    property alias selectRole: roleSelectionView.currentIndex

    spacing: 0

    Button {
        id: backBtn
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.preferredWidth: 40 / Style.monitorRatio
        Layout.topMargin: 30 / Style.monitorRatio
        background: Image {
            source: "qrc:/Resources/back.png"
        }
    }

    RowLayout {
        id: header
        Layout.fillWidth: true
        Layout.topMargin: 30 / Style.monitorRatio

        Text {
            id: titleText
            text: "Log in"
            font.pixelSize: 35 / Style.monitorRatio
            color: Style.foregroundColor
            Layout.fillWidth: true
        }

        Button {
            id: connectionStatus
            background: Image {
                source: connectionConfigCpp.isConnected ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
            }
            Layout.preferredHeight: 39 / Style.monitorRatio
            Layout.preferredWidth: 39 / Style.monitorRatio

            onClicked: {

            }
        }
    }

    RowLayout {
        spacing: 0
        Layout.topMargin: 50 / Style.monitorRatio
        IconImage {
            source: "qrc:/Resources/user.png"
            Layout.preferredHeight: 25 / Style.monitorRatio
            Layout.preferredWidth: 25 / Style.monitorRatio
        }

        Text {
            id: usernameTxt
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

    ListView {
        id: roleSelectionView
        model: roleSelectionModel
        currentIndex: 0
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 90 / Style.monitorRatio
        Layout.fillWidth: true
        spacing: 10 / Style.monitorRatio
        interactive: false
        delegate: RadioButton {
            id: radioButton
            checked: roleSelectionView.currentIndex === index
            text: model.display
            height: 20 / Style.monitorRatio
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
                source: radioButton.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
            }

            onCheckedChanged: {
                if (radioButton.checked)
                    roleSelectionView.currentIndex = index
            }
        }
    }

    RowLayout {
        spacing: 0
        Layout.topMargin: 12 / Style.monitorRatio
        opacity: (userManager.message === "") ? 0 : 1
        Image {
            source: "qrc:/Resources/error.png"
            Layout.preferredHeight: 20 / Style.monitorRatio
            Layout.preferredWidth: 20 / Style.monitorRatio
        }
        Text {
            text: userManager.message
            font.pixelSize: 17 / Style.monitorRatio
            color: "#ED4337"
            Layout.leftMargin: 5 / Style.monitorRatio
        }
    }
    Button {
        id: signInBtn
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.fillWidth: true
        Layout.topMargin: 15 / Style.monitorRatio
        hoverEnabled: true
        background: Rectangle {
            color: Style.foregroundColor
            radius: width / (Style.monitorRatio * 2)
        }
        contentItem: Text {
            text: "Sign in"
            color: parent.hovered
                   && parent.enabled ? "#01AED6" : Style.backgroundColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {

        }
    }
}
