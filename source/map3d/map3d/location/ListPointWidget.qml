import QtQuick 2.0
import Qt.labs.platform 1.1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import "../style"
Item {
    id :rootItem
    signal goToView(string name, real latitude, real longitude,
                    real range, real pitch ,real head);



    Rectangle{
        anchors.fill: parent
        color: Style.foregroundColor
        radius: Style.radius
        opacity: 0.8
    }
    ScrollView {
        id: scroller
        anchors.fill: parent
        contentHeight: columnLayout.implicitHeight
        clip : true
        Item {
            id: tmp
            anchors.fill: parent
        }
        Column {
            id :columnLayout
            anchors.fill: parent
            spacing: 2
            Repeater {
                id: repeater
                model: listSaveLocation
                delegate: Button{
                    checked: false
                    id:subbutton
                    width: columnLayout.width
                    height: 30
                    text: _Name

                    hoverEnabled: true
                    display: AbstractButton.TextOnly
                    font.family: _fontFamily
                    font.pointSize: _fontPointSize - 1
                    opacity: 1
                    onClicked: function() {
                            rootItem.goToView(_Name,
                                            parseFloat(_Latitude),
                                            parseFloat(_Longitude),
                                            parseFloat(_Range),
                                            parseFloat(_Pitch),
                                            parseFloat(_Head)
                                              )
                    }
                    contentItem:
                            Text {
                                id:subtxt
                                text: subbutton.text
                                font.family: subbutton.font.family
                                font.pointSize:  subbutton.font.pointSize
                                color: hovered || checked  ? (pressed ? _colorPresed: _colorHover) :
                                                             (pressed ? _colorHover : "#FFFFFF");
                                anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                opacity: 1
                            }
                    background: Rectangle {
                        color:  "transparent"
                        radius: _radius
                        opacity: 1
                    }

                }
            }
        }
    }
}
