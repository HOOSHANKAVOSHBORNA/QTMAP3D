import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12






Item {
    property double step: 1
    property double value
    property double showText   : value ? value : 0
    property string secondaryColor: "#ffb300"
    property string primaryColor  : "#c9c9c9"
    property string btntxtColor   : "#404040"
    property int    spinSpeed     : 50
    property int    decimals      : 3
    property double from          : 0
    property double to            : 9999999999999


    id: btn
    width: 120
    height: 30


    Timer {
        id: timerUp
        interval: spinSpeed; running: false; repeat: true
        onTriggered: if(value < to){
                         value += step
                     }
    }
    Timer {
        id: timerDown
        interval: spinSpeed; running: false; repeat: true
        onTriggered: if(value > from){
                         value -= step
                     }
    }


    Rectangle{
        id:mainHolder
        anchors.fill: parent
        color: btntxtColor
        radius: 3
        border.color: btntxtColor
        border.width:  1




        Rectangle{
            id:stepContainer
            width: Math.min(btn.width/4 , btn.height -4 )
            height: btn.height -4
            anchors.verticalCenter: parent.verticalCenter
            //            anchors.left: parent.left
            x:2
            color: btntxtColor
            radius: 3
            border.color: primaryColor
            border.width: 1
            ColumnLayout{
                anchors.centerIn: parent
                Text {
                    id:stepText
                    text: qsTr("STEP")
                    font.pixelSize: stepContainer.height/3
                    color: primaryColor
                }
                Text {
                    id:stepSize
                    text: step.toString()
                    font.pixelSize: btn.height/4
                    color: primaryColor
                }
            }
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
                onPressed:  if(dropDownMenu.height === 0){
                                openStepAnimate.running = true
                                dropDownMenuTextopen.running = true
                            }else{
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                onEntered: stepText.color = secondaryColor
                onExited: stepText.color = primaryColor
            }
            Rectangle{
                id: dropDownMenu
                width: btn.width
                z: 10
                height: 0
                anchors.top: parent.bottom
                color: "transparent"

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onExited: {
                        closeStepAnimate.running = true
                        dropDownMenuTextclose.running = true
                    }
                }

                Grid{
                    anchors.margins: 1
                    columns: 4
                    rows: 2
                    spacing: 1
                    anchors.fill: parent
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:kilo
                            text: qsTr("1000")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:  1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 1000
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: kilo.color = secondaryColor
                            onExited: kilo.color = btntxtColor

                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:sad
                            text: qsTr("100")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 100
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: sad.color = secondaryColor
                            onExited: sad.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:dah
                            text: qsTr("10")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 10
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: dah.color = secondaryColor
                            onExited: dah.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:panj
                            text: qsTr("5")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 5
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: panj.color = secondaryColor
                            onExited: panj.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:yek
                            text: qsTr("1")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 1
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: yek.color = secondaryColor
                            onExited: yek.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:dahom
                            text: qsTr("0.1")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 0.1
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: dahom.color = secondaryColor
                            onExited: dahom.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:sadom
                            text: qsTr("0.01")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 0.01
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: sadom.color = secondaryColor
                            onExited: sadom.color = btntxtColor
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        border.color: btntxtColor
                        radius: Math.min(width/8 , height/8)
                        Text {
                            id:hezarom
                            text: qsTr("0.001")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                step = 0.001
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                            hoverEnabled: true
                            onEntered: hezarom.color = secondaryColor
                            onExited: hezarom.color = btntxtColor
                        }
                    }
                }
            }
        }
        Rectangle{
            id: plus
            anchors.verticalCenter: parent.verticalCenter
            height: mainHolder.height
            width: mainHolder.width/9
            //            radius: mainHolder.height/8
            color: btntxtColor
            anchors.right: mainHolder.right
            //            border.width: 2
            //            border.color: primaryColor
            Text {
                id:plusText
                text: qsTr("+")
                color: primaryColor
                font.pixelSize: plus.width/1.3
                anchors.centerIn: parent
            }
            MouseArea{
                cursorShape: "PointingHandCursor"
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    plusText.color = secondaryColor
                }
                onExited: {
                    plusText.color = primaryColor
                }

                onPressed:  {
                    if(value < from){value = from}
                    else if(value > to){value = to}
                    else if(value >= from && value <to){
                        plus.color = secondaryColor
                        value += step
                    }
                }
                onPressAndHold: timerUp.start();
                onReleased: {
                    plus.color = btntxtColor
                    timerUp.stop();
                }
            }
        }
        Rectangle{
            id: minus
            anchors.verticalCenter: parent.verticalCenter
            height: mainHolder.height
            width: mainHolder.width/9
            //            radius: mainHolder.height/8
            color: btntxtColor
            anchors.left: stepContainer.right
            //            border.width: 2
            //            border.color: primaryColor
            Text {
                id: minusText
                text: qsTr("-")
                color: primaryColor
                font.pixelSize: minus.width*1.1
                anchors.centerIn: parent
            }
            MouseArea{
                cursorShape: "PointingHandCursor"
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    minusText.color = secondaryColor
                }
                onExited: {
                    minusText.color = primaryColor
                }
                onPressed:  {
                    if(value < from){value = from}
                    else if(value > to){value = to}
                    else if(value > from && value <= to){
                        minus.color = secondaryColor
                        value -= step
                    }
                }
                onPressAndHold: timerDown.start();
                onReleased: {
                    minus.color = btntxtColor
                    timerDown.stop();
                }
            }
        }

        Rectangle {
            id: valueHolder
            anchors.left: minus.right
            anchors.right: plus.left
            height: parent.height
            color: primaryColor
            clip: true
            border.width: 1
            border.color: btntxtColor
            TextInput {
                id: valueText
                x: 4
                //                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                onTextChanged: value = showText
                text: showText.toFixed(decimals)
                font.pixelSize: parent.height/3
                color: btntxtColor
                selectByMouse: true
                selectedTextColor: "black"
                selectionColor: secondaryColor
            }
        }
    }
    PropertyAnimation{
        id: openStepAnimate
        target: dropDownMenu
        property: "height"
        to : btn.height*2
        duration: 100
        running: false
    }
    PropertyAnimation{
        id: closeStepAnimate
        target: dropDownMenu
        property: "height"
        to : 0
        duration: 100
        running: false
    }
    PropertyAnimation{
        id:dropDownMenuTextopen
        targets: [kilo,sad,dah,yek,dahom,sadom,hezarom,panj]
        properties: "font.pixelSize"
        to: btn.height/4
        duration: 200
        running: false
    }
    PropertyAnimation{
        id:dropDownMenuTextclose
        targets: [kilo,sad,dah,yek,dahom,sadom,hezarom,panj]
        properties: "font.pixelSize"
        to: 1
        duration: 100
        running: false
    }
}
