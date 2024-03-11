import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

Rectangle {
    // property var loadingPageCpp: undefined
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
            anchors.topMargin: 30
            anchors.bottomMargin: 30
            spacing: 50

            Image {
                source: "qrc:/Resources/Qarch.png"
                Layout.preferredWidth: 100 / Style.monitorRatio
                Layout.preferredHeight: 100 / Style.monitorRatio
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                id: loadingContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 200 / Style.monitorRatio
                color: "transparent"
                clip: true
                ListView {
                    id: listView
                    model: applicationCpp.loadingPageCpp ? applicationCpp.loadingPageCpp : 0
                    anchors.fill: parent
                    verticalLayoutDirection: ListView.BottomToTop
                    onCountChanged: {
                        progressBar.value = loadingPageCpp.rowCount(
                                    ) / loadingPageCpp.pluginCounter
                    }

                    delegate: Text {
                        id: loadingText

                        font.pixelSize: Style.smallFontSize
                        color: errorStatus ? "white" : "red"

                        text: display
                        horizontalAlignment: Text.AlignHCenter
                        width: listView.width
                    }

                    //                    populate: Transition {
                    //                        NumberAnimation {
                    //                            properties: "x,y"
                    //                            duration: 10000
                    //                        }
                    //                    }

                    //                    add: Transition {
                    //                        NumberAnimation {
                    //                            property: "opacity"
                    //                            from: 0
                    //                            to: 1.0
                    //                            duration: 200
                    //                        }
                    //                        NumberAnimation {
                    //                            property: "scale"
                    //                            from: 0
                    //                            to: 1.0
                    //                            duration: 200
                    //                        }
                    //                    }

                    //                    move: Transition {
                    //                        NumberAnimation {
                    //                            properties: "x,y"
                    //                            duration: 200
                    //                        }
                    //                    }

                    //                    remove: Transition {
                    //                        NumberAnimation {
                    //                            properties: "x,y"
                    //                            duration: 200
                    //                        }
                    //                    }

                    //                    removeDisplaced: Transition {
                    //                        NumberAnimation {
                    //                            properties: "x,y"
                    //                            duration: 200
                    //                        }
                    //                    }
                    //                    displaced: Transition {
                    //                        NumberAnimation {
                    //                            properties: "x,y"
                    //                            duration: 200
                    //                        }
                    //                    }
                }
            }

            ProgressBar {
                id: progressBar
                value: 0
                padding: 2
                Layout.alignment: Qt.AlignHCenter
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
        }
    }
}
