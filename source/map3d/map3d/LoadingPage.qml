import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    property var loadingPageCpp: undefined
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
            anchors.fill: parent

            Image {
                source: "qrc:/Resources/Qarch.png"
                Layout.preferredWidth: 100 / Style.monitorRatio
                Layout.preferredHeight: 100 / Style.monitorRatio
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                id: loadingContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 100 / Style.monitorRatio
                color: "transparent"
                clip: true
                ScrollView {
                    anchors.fill: parent
                    ListView {
                        id: listView
                        model: loadingPageCpp
                        anchors.fill: parent
                        delegate: Text {
                            id: loadingText
                            font.pixelSize: 20 / Style.monitorRatio
                            color: acceptionState ? "white" : "red"
                            text: display
                            horizontalAlignment: Text.AlignHCenter
                            width: listView.width
                        }

                        populate: Transition {
                            NumberAnimation {
                                properties: "x,y"
                                duration: 10000
                            }
                        }

                        add: Transition {
                            NumberAnimation {
                                property: "opacity"
                                from: 0
                                to: 1.0
                                duration: 200
                            }
                            NumberAnimation {
                                property: "scale"
                                from: 0
                                to: 1.0
                                duration: 200
                            }
                        }

                        move: Transition {
                            NumberAnimation {
                                properties: "x,y"
                                duration: 200
                            }
                        }

                        remove: Transition {
                            NumberAnimation {
                                properties: "x,y"
                                duration: 200
                            }
                        }

                        removeDisplaced: Transition {
                            NumberAnimation {
                                properties: "x,y"
                                duration: 200
                            }
                        }
                        displaced: Transition {
                            NumberAnimation {
                                properties: "x,y"
                                duration: 200
                            }
                        }
                    }
                }
            }
            AnimatedImage {
                source: "qrc:/Resources/loading.gif"
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 40 / Style.monitorRatio
                Layout.preferredWidth: 40 / Style.monitorRatio
            }
        }
    }
}
