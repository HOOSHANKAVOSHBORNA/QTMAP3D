import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtQuick.Effects
import Crystal 1.0
import "../style"

Rectangle{

    signal goToLocation(real latitude , real longitude, real range)


    id:root
    color: Style.primaryColor
    radius: Style.radius
    opacity: 0.8
    height:columnGo.implicitHeight * 1.3
    width: parent.width / 2
    ListModel{
        id : listelemnt
        ListElement{
            placeholdername: "latitude"
            rangebottem : -180.0
            rangetop : 180.0
        }
        ListElement{
            placeholdername: "longitude"
            rangebottem : -90.0
            rangetop : 90.0
        }
        ListElement{
            placeholdername: "Range"
            rangebottem : 0
            rangetop : 5000000
        }
    }

    Column{
        id :columnGo
        spacing: 3
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: Style.marginSize / 5
            rightMargin: Style.marginSize / 5
            topMargin : Style.marginSize
        }
        Repeater{
            id:repeter
            model:listelemnt
            delegate: TextField{
                height: 30
                width: parent.width
                anchors.topMargin: Style.marginSize
                placeholderText: placeholdername
                hoverEnabled : true
                font.family: Style.fontFamily
                font.pointSize: Style.fontPointSize
                color: "#FFFFFF"
                onPressed:{
                    text = ""
                }

                validator:  DoubleValidator {bottom:rangebottem ; top: rangetop ; decimals:5 }
                background: Rectangle{
                    color: "#404040"
                    radius: 10
                    opacity: 0.3
                }


            }
        }
        Button{
            id:btnGo
            height: 30
            hoverEnabled: true
            width: parent.width - (5 * 2 )
            anchors.horizontalCenter: parent.horizontalCenter

            text: "Go To"
            font.family: Style.fontFamily
            font.pointSize: Style.fontPointSize
            onClicked:{
                if(repeter.itemAt(0).text !=="" &&
                   repeter.itemAt(1).text !=="" &&
                   repeter.itemAt(2).text !==""){
                    root.goToLocation(parseFloat(repeter.itemAt(0).text),
                                      parseFloat(repeter.itemAt(1).text),
                                      parseFloat(repeter.itemAt(2).text))
                }
            }

            contentItem:Text {
                id:txt
                text: btnGo.text
                font.family: btnGo.font.family
                font.pointSize:  btnGo.font.pointSize

                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                opacity: 1
            }
            background:Rectangle {
                color: Style.hoverColor
                opacity: btnGo.hovered ? 0.8 : 1
                radius: Style.radius
                layer.enabled: true
//                layer.effect: DropShadow {
//                    transparentBorder: true
//                    horizontalOffset: 7
//                    verticalOffset: 7
//                    color: "#88000000"
//                }


            }
        }

    }
}

