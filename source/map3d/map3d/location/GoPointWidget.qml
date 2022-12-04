import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13

Rectangle{

    signal goToLocation(real latitude , real longitude, real range)


    id:root
    color: _colorRec
    radius: _radius
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
            leftMargin: _margin / 5
            rightMargin: _margin / 5
            topMargin : _margin
        }
        Repeater{
            id:repeter
            model:listelemnt
            delegate: TextField{
                height: 30
                width: parent.width
                anchors.topMargin: _margin
                placeholderText: placeholdername
                hoverEnabled : true
                font.family: _fontFamily
                font.pointSize: _fontPointSize
                color: "#FFFFFF"
                onPressed:{
                    text = ""
                }

                validator:  DoubleValidator {bottom:rangebottem ; top: rangetop ; decimals:5 }
                background: Rectangle{
                    color: _colorButton
                    radius: _radius
                    opacity: 0.3
                }


            }
        }
        Button{
            id:btnGo
            height: 30
            hoverEnabled: true
            width: parent.width - (_margin * 2 )
            anchors.horizontalCenter: parent.horizontalCenter

            text: "Go To"
            font.family: _fontFamily
            font.pointSize: _fontPointSize
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
                color: _colorHover
                opacity: btnGo.hovered ? 0.8 : 1
                radius: _radius
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 7
                    verticalOffset: 7
                    color: "#88000000"
                }


            }
        }

    }
}

