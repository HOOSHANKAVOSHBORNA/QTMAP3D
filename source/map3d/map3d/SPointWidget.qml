import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13

Rectangle{

    property real longtitude: 0.0
    property real latitude: 0.0
    signal savePointClicked(real latitude , real longtitude, real range)


    color: _colorRec
    radius: _radius
    opacity: 0.8
    height:columnGo.implicitHeight * 1.3
    width: parent.width
    Column{
        id :columnGo
        spacing: 3
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: _margin
        }
        TextField{
            height: 30
            width: parent.width - _margin
            anchors.topMargin: _margin
            placeholderText: "name"
            hoverEnabled : true
            font.family: _fontFamily
            font.pointSize: _fontPointSize
            color: "black"
            validator: IntValidator {bottom: -90; top: 90}
            background: Rectangle{
                radius: _radius
                border.width: 1
                border.color: pressAndHold ? _colorHover : "#FFFFFF"
            }


        }
        Repeater{
            id:repeter
            model:["Lon :" + longtitude  ,"Lat :" + latitude]
            delegate: Label{
                height: 30
                width: parent.width - _margin
                anchors.topMargin: _margin
                text: modelData
                color: "black"
                padding : _margin
                font.family: _fontFamily
                font.pointSize: _fontPointSize
                verticalAlignment : Text.AlignVCenter
                background: Rectangle{
                    radius: _radius
                    border.width: 1
                    border.color: _colorHover
                }


            }
        }
        Button{
            id:btnGo
            height: 30
            hoverEnabled: true
            width: parent.width - _margin
            text: "Save Point"
            font.family: _fontFamily
            font.pointSize: _fontPointSize
            onClicked:{
                savePointClicked(repeter.itemAt(0).text, repeter.itemAt(1).text, repeter.itemAt(2).text)
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

