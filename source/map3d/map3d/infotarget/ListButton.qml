import QtQuick 2.12
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
Item {

    property var  listbutton:ListModel{

    }


    //height: 50
    anchors.horizontalCenter: parent.horizontalCenter

    Row {
        id: row
        anchors.fill: parent
        spacing: 30
        Repeater{
            anchors.fill: parent
            model:listbutton
            delegate: Button{
                checked: false
                id:button
                width: 45
                height: 45
                text: buttonName
                hoverEnabled: true
                display: AbstractButton.TextBesideIcon
                icon.source: buttonIcon
                icon.width: 24
                icon.height: 24
                font.family: _fontFamily
                font.pointSize: _fontPointSize - 3
                opacity: 1

                contentItem:Item {
                    Column{
                        anchors.fill: parent
                        spacing: 1
                        Image {
                            id: img
                            source: button.icon.source
                            width: button.icon.width
                            height: button.icon.height
                            //visible: false
                            //anchors.verticalCenter: parent.verticalCenter
                            ColorOverlay {
                                    anchors.fill: img
                                    source: img
                                    color: hovered   ? (pressed ? _colorPresed: _colorHover) :
                                                       (pressed ? _colorHover : "#FFFFFF");
                                }

                        }


                        Text {
                            id:subtxt
                            text: button.text
                            font.family: button.font.family
                            font.pointSize:  button.font.pointSize
                            color: hovered   ? (pressed ? _colorPresed: _colorHover) :
                                                         (pressed ? _colorHover : "#FFFFFF");
                            //anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            opacity: 1
                        }
                    }
                }


                background: Rectangle {
                    color:  "transparent"

                }

            }
        }
    }


}

/*##^##
Designer {
    D{i:2;anchors_height:400;anchors_width:200;anchors_x:75;anchors_y:14}
}
##^##*/
