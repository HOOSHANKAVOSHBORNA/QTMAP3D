import QtQuick 2.13
import "style"

Item {
    property var loadingPageCpp: undefined

    Rectangle {
        id: loadingContainer
        width: parent.width
        height: 0.1 * parent.height

        color: "pink"
        clip: true

        ListView {
            id: listView
            model: loadingPageCpp
            height: parent.height
            width: 0.05 * parent.width
            anchors.horizontalCenter: parent.horizontalCenter
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
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100 / Style.monitorRatio
        anchors.horizontalCenter: parent.horizontalCenter
        width: 40 / Style.monitorRatio
        height: 40 / Style.monitorRatio
    }
}
