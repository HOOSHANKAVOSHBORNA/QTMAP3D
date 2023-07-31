import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtQuick.Effects
import Qt.labs.platform 1.1
import "../style"
Rectangle{
    id : rootItem
    property real longitude: 0.0
    property real latitude: 0.0
    property real pitch: 0.0
    property real range: 0.0
    property real head: 0.0

    signal saveLocation(string name, real latitude, real longitude,
                       real pitch ,real range, real head)

    color: Style.primaryColor
    radius: Style.radius
    opacity: 0.8
    height:columnGo.implicitHeight * 1.2
    width: parent.width / 2






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
        TextField{
            id:nameTextfeild
            height: 30
            width: parent.width
            anchors.topMargin: Style.marginSize
            placeholderText: "name"
            hoverEnabled : true
            font.family: Style.fontFamily
            font.pointSize: Style.fontPointSize
            color: "#FFFFFF"
            background: Rectangle{
                color: Style.backgroundColor
                radius: Style.radius
                opacity: 0.3
                width: parent.width
            }
            onPressed: {
                text = ""
            }


        }
        Rectangle{
            color:Qt.lighter(Style.primaryColor,.5) //_colorButton
            //opacity: 0.3
            radius: Style.radius
            height: parent.width  / 1.2
            width: parent.width
            Column{
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: Style.marginSize / 2
                    bottomMargin: Style.marginSize / 2
                    leftMargin: 2
                    rightMargin: 2
                }
                spacing: 3
                Repeater{
                    id:repeter
                    model:[
                        "Lat: " +    Number(latitude).toLocaleString(Qt.locale(), 'f', 2),
                        "Lon: " + Number(longitude).toLocaleString(Qt.locale(), 'f', 3),
                        "Range: " +  Number(range).toLocaleString(Qt.locale(), 'e', 2),
                        "Pitch: " +  Number(pitch).toLocaleString(Qt.locale(), 'f', 3),
                        "Head: " +  Number(head).toLocaleString(Qt.locale(), 'f', 3)
                    ]
                    delegate: Label{
                        height: 20
                        width: parent.width
                        anchors.topMargin: Style.marginSize
                        text: modelData
                        color: "#EEFFFFFF"
                        padding: 1
                        font.family: Style.fontFamily
                        font.pointSize: Style.fontPointSize
                        verticalAlignment : Text.AlignVCenter
                        opacity: 1
                    }
                }

            }

        }


        Button{
            id:btnGo
            height: 30
            hoverEnabled: true
            width: parent.width - (5 * 2)
            text: "Save"
            anchors.horizontalCenter: nameTextfeild.horizontalCenter
            font.family: Style.fontFamily
            font.pointSize: Style.fontPointSize

            onClicked:{
                if (nameTextfeild.text !== ""){
                rootItem.saveLocation(nameTextfeild.text,
                                      parseFloat(repeter.itemAt(0).text.split(":")[1]),
                                      parseFloat(repeter.itemAt(1).text.split(":")[1]),
                                      parseFloat(repeter.itemAt(2).text.split(":")[1]),
                                      parseFloat(repeter.itemAt(3).text.split(":")[1]),
                                      parseFloat(repeter.itemAt(4).text.split(":")[1])
                            )
            }
                //savePointClicked(nameTextfeild.text, , repeter.itemAt(1).text.split(":")[1])
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

