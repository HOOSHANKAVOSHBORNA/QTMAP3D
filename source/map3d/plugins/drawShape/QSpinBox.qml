import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects


Item {

    property var steps: [5000,1000,100,10,5,1,0.5,0.1,0.05,0.01,0.005,0.001]
    property int index: 5
    property double value
    property string secondaryColor: "orange"
    property string btntxtColor   : "#c9c9c9"
    property string primaryColor   : "#424242"
    property double showText
    property int    spinSpeed     : 30
    property int    decimals
    property double from          : -9999999999999
    property double to            : 9999999999999
    property bool round: true

    onShowTextChanged: value = showText

    implicitHeight: 35
    implicitWidth: 200

    Rectangle{
        height: parent.height
        width: parent.width
        color: btntxtColor
        border.width: 1
        border.color: primaryColor
        radius: round ? height/4 : 0

        Timer {
            id: timerUp
            interval: spinSpeed; running: false; repeat: true
            onTriggered: if(value < to){
                             value += steps[index]
                         }
        }
        Timer {
            id: timerDown
            interval: spinSpeed; running: false; repeat: true
            onTriggered: if(value > from){
                             value -= steps[index]
                         }
        }

        Rectangle{
            id: stepContainer
            width: parent.width * 0.35 -4
            height: parent.height -4
            x:2
            y:2
            color: primaryColor
            clip: true
            radius: round ? height/4 : 0

            Rectangle{
                anchors.right: parent.right
                height: parent.height
                width: parent.width * 0.7
                color: "transparent"
                Text {
                    id: stepperValue
                    text: steps[index]
                    anchors.margins: 1
                    color: btntxtColor
                    font.pixelSize:   parent.height ? parent.height/2.5 : 0
                    anchors.centerIn: parent
                    rotation: 0
                }
            }
            Image {
                id: stepUp
                source: "./resources/arrow.png"
                sourceSize.height: parent.height/2.1
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: sourceSize.height/10
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onEntered: upColor.colorizationColor = secondaryColor
                    onExited: upColor.colorizationColor = btntxtColor
                    onPressed: if(index >0){index --}
                }
            }
            MultiEffect{
                id:upColor
                anchors.fill: stepUp
                source: stepUp
                colorizationColor: btntxtColor
                colorization: 1
                visible: true
            }

            Image {
                id: stepDown
                source: "./resources/arrow.png"
                rotation: 180
                sourceSize.height: parent.height/2.1
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: sourceSize.height/10
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onEntered: downColor.colorizationColor = secondaryColor
                    onExited: downColor.colorizationColor = btntxtColor
                    onPressed: if(index < steps.length-1){index ++}
                }
            }
            MultiEffect{
                id:downColor
                anchors.fill: stepDown
                source: stepDown
                colorizationColor: btntxtColor
                colorization: 1
                visible: true
                rotation: stepDown.rotation
            }
        }

        Rectangle {
            id: spinnerContainer
            height: parent.height -2
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.65 -4
            anchors.right: parent.right
            //            border.width: 1
            //            border.color: primaryColor
            color: btntxtColor
            radius: round ? height/4 : 0
            Image {
                id: spinUp
                source: "./resources/arrow.png"
                sourceSize.height: parent.height/2.1
                anchors.right:  parent.right
                anchors.top: parent.top
                anchors.margins: sourceSize.height/10
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onEntered: spinupColor.colorizationColor = secondaryColor
                    onExited: spinupColor.colorizationColor = primaryColor
                    onPressed:  {
                        if(value < from){value = from}
                        else if(value > to){value = to}
                        else if(value >= from && value <to){
                            spinupColor.colorizationColor = btntxtColor
                            value += steps[index]
                        }
                    }
                    onPressAndHold: timerUp.start();
                    onReleased: {
                        spinupColor.colorizationColor = secondaryColor
                        timerUp.stop();
                    }
                }
            }
            MultiEffect{
                id:spinupColor
                anchors.fill: spinUp
                source: spinUp
                colorizationColor: primaryColor
                colorization: 1
                visible: true

            }

            Image {
                id: spinDown
                source: "./resources/arrow.png"
                rotation: 180
                sourceSize.height: parent.height/2.1
                anchors.right:  parent.right
                anchors.bottom: parent.bottom
                anchors.margins: sourceSize.height/10
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true
                    onEntered: spindownColor.colorizationColor = secondaryColor
                    onExited: spindownColor.colorizationColor = primaryColor
                    onPressed:  {
                        if(value < from){value = from}
                        else if(value > to){value = to}
                        else if(value > from && value <= to && value > steps[index]){
                            spindownColor.colorizationColor = btntxtColor
                            value -= steps[index]
                        }
                        else if (steps[index] > value){
                            value = from
                        }

                    }
                    onPressAndHold: timerDown.start();
                    onReleased: {
                        spindownColor.colorizationColor = secondaryColor
                        timerDown.stop();
                    }
                }
            }
            MultiEffect{
                id:spindownColor
                anchors.fill: spinDown
                source: spinDown
                colorizationColor: primaryColor
                colorization: 1
                visible: true
                rotation: spinDown.rotation

            }
            Rectangle{
                height: parent.height -2
                width: parent.width - 1.2*spinUp.width
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                clip: true
                color: "transparent"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "IBeamCursor"
                }

                TextInput {
                    id: input
                    text: value ? value.toFixed(decimals) : 0
                    padding: 5
                    anchors.fill: parent
                    color: primaryColor
                    selectByMouse: true
                    selectionColor: secondaryColor
                    selectedTextColor: primaryColor
                    font.pixelSize: parent.height ? parent.height*0.6 : 0
                    onTextEdited: value = parseFloat(text)
                    mouseSelectionMode: TextInput.SelectCharacters
                    autoScroll: true
                    inputMethodHints: "ImhFormattedNumbersOnly"

                }
            }
        }
    }
}
