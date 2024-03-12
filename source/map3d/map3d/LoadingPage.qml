import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    property var loadingPageCpp: applicationCpp.loadingPageCpp
    color: "transparent"
    Rectangle {
        width: parent.width / 2
        height: parent.height
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(Style.foregroundColor.r,
                               Style.foregroundColor.g,
                               Style.foregroundColor.b, 0.3)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(Style.foregroundColor.r,
                               Style.foregroundColor.g,
                               Style.foregroundColor.b, 0.1)
            }
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 0
            Image {
                source: "qrc:/Resources/Qarch.png"
                Layout.preferredWidth: 150 / Style.monitorRatio
                Layout.preferredHeight: 150 / Style.monitorRatio
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 344 / Style.monitorRatio
            }

            ProgressBar {
                id: progressBar
                value: 0
                padding: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 30 / Style.monitorRatio
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 8
                    color: "#D9D9D9"
                    radius: 5 / Style.monitorRatio
                }
                contentItem: Item {
                    implicitWidth: 200 / Style.monitorRatio
                    implicitHeight: 8 / Style.monitorRatio

                    Rectangle {
                        width: progressBar.visualPosition * parent.width
                        height: parent.height
                        radius: 5 / Style.monitorRatio
                        color: "#01AED6"
                    }
                }
            }

            Text {
                id: loadingText
                font.pixelSize: Style.regularFontSize
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 30 / Style.monitorRatio
                Connections {
                    target: loadingPageCpp
                    function onItemAdded() {
                        loadingText.text = loadingPageCpp.firstItem()
                        loadingText.color
                                = loadingPageCpp ? (loadingPageCpp.firstItemErrorStatus(
                                                        ) ? "red" : "white") : "transparent"
                        progressBar.value = loadingPageCpp ? loadingPageCpp.pluginFraction : 0
                    }
                }
            }
        }
    }
}
