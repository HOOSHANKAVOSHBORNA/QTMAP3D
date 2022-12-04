import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13

Rectangle{
    id:root
    color: _colorRec
    radius: _radius
    opacity: 0.8
    height:columnGo.implicitHeight * 1.3
    signal goToLocation(real latitude , real longtitude, real range)
    width: parent.width / 2
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
        Repeater{
            id:repeter
            model:["latitude" ,"longtitude","Range"]
            delegate: TextField{
                height: 30
                width: parent.width
                anchors.topMargin: _margin
                placeholderText: modelData
                hoverEnabled : true
                font.family: _fontFamily
                font.pointSize: _fontPointSize
                color: "#FFFFFF"
                validator:  DoubleValidator {bottom: -90.0; top: 90.0}
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
                root.goToLocation(parseFloat(repeter.itemAt(0).text), parseFloat(repeter.itemAt(1).text), parseFloat(repeter.itemAt(2).text))
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

