import QtQuick
import QtQuick.Layouts
import "style"

ColumnLayout {
    property var loadingPageCpp: undefined

    Rectangle {
        id: loadingContainer
        //        width: parent.width
        //        height: 0.1 * parent.height
        Layout.fillWidth: true
        Layout.preferredHeight: 70 / Style.monitorRatio

        color: "pink"
        clip: true

        ListView {
            id: listView
            model: loadingPageCpp
            //            height: parent.height
            //            width: 0.05 * parent.width
            //            anchors.horizontalCenter: parent.horizontalCenter
            anchors.fill: parent
            delegate: Text {
                id: loadingText
                font.pixelSize: 20 / Style.monitorRatio
                color: acceptionState ? "white" : "green"
                text: display
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
    AnimatedImage {
        source: "qrc:/Resources/loading.gif"

        //        anchors.horizontalCenter: parent.horizontalCenter
        Layout.alignment: Qt.AlignHCenter
        //        width: 40 / Style.monitorRatio
        //        height: 40 / Style.monitorRatio
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.preferredWidth: 40 / Style.monitorRatio
    }
}
