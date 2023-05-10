import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


Item {
    property double step: 1
    property double value: 256
    property string secondaryColor: "orange"
    property string primaryColor  : "#454545"
    property string btntxtColor   : "#bbbbbb"
    id: btn
    width: 120
    height: 30
    Rectangle{
        id:mainHolder
        anchors.fill: parent
        color: primaryColor
        radius: 3
        border.color: btntxtColor
        border.width: 2
        Rectangle{
            id:stepContainer
            width: btn.width/4
            height: btn.height -4
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            color: primaryColor
            radius: 3
            ColumnLayout{
                anchors.centerIn: parent
                Text {
                    id:stepText
                    text: qsTr("STEP")
                    font.pixelSize: btn.height/4
                    color: btntxtColor
                }
                Text {
                    id:stepSize
                    text: step.toString()
                    font.pixelSize: btn.height/4
                    color: btntxtColor
                }
            }
            MouseArea{
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
                onPressed:  if(dropDownMenu.height === 0){
                               openStepAnimate.running = true
                               dropDownMenuTextopen.running = true
                                stepText.color = secondaryColor
                           }else{
                               closeStepAnimate.running = true
                               dropDownMenuTextclose.running = true
                           }
                onReleased: stepText.color = btntxtColor
            }
            Rectangle{
                id: dropDownMenu
                width: btn.width
                height: 0
                anchors.top: parent.bottom
                color: "transparent"

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
                        radius: 3
                        Text {
                            id:kilo
                            text: qsTr("1000")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:  1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 1000
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:sad
                            text: qsTr("100")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 100
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:dah
                            text: qsTr("10")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 10
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:panj
                            text: qsTr("5")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 5
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:yek
                            text: qsTr("1")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 1
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:dahom
                            text: qsTr("0.1")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize:1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 0.1
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:sadom
                            text: qsTr("0.01")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 0.01
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                    Rectangle{
                        width: dropDownMenu.width/4   -1
                        height: dropDownMenu.height/2 -1
                        color: primaryColor
                        radius: 3
                        Text {
                            id:hezarom
                            text: qsTr("0.001")
                            anchors.centerIn: parent
                            color: btntxtColor
                            font.pixelSize: 1
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                step = 0.001
                                closeStepAnimate.running = true
                                dropDownMenuTextclose.running = true
                            }
                        }
                    }
                }
            }
        }
        Rectangle{
            id: plus
            anchors.verticalCenter: parent.verticalCenter
            height: mainHolder.height -4
            width: mainHolder.width/7
            radius: mainHolder.height/8
            color: btntxtColor
            anchors.right: mainHolder.right
            border.width: 4
            border.color: primaryColor
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
                onPressed:  {
                    plusText.color = secondaryColor
                    value += step
                }
                onReleased: {
                    plusText.color = primaryColor
                }
            }
        }
        Rectangle{
            id: minus
            anchors.verticalCenter: parent.verticalCenter
            height: mainHolder.height -4
            width: mainHolder.width/7
            radius: mainHolder.height/8
            color: btntxtColor
            anchors.left: stepContainer.right
            border.width: 4
            border.color: primaryColor
            Text {
                id: minusText
                text: qsTr("-")
                color: "#525252"
                font.pixelSize: minus.width*1.1
                anchors.centerIn: parent
            }
            MouseArea{
                cursorShape: "PointingHandCursor"
                anchors.fill: parent
                onPressed:  {
                    minusText.color = secondaryColor
                    value -= step
                }
                onReleased: {
                    minusText.color = primaryColor
                }
            }
        }

        Rectangle {
            id: valueHolder
            anchors.left: minus.right
            anchors.right: plus.left
            height: parent.height
            color: "transparent"
            clip: true
            TextInput {
                id: valueText
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: value.toFixed(6)
                font.pixelSize: parent.height/3
                color: btntxtColor
                onTextChanged: value = text
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
