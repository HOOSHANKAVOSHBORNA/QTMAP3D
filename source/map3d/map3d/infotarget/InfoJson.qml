import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.13

Item {
    property string jsonString: ""



    width: parent.width - _margin
    anchors.horizontalCenter: parent.horizontalCenter
    height: 300
    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: _colorRec
        radius: _radius
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 7
            verticalOffset: 7
            color: "#88000000"
            Flickable {
                id: flickable
                anchors.fill: parent

                TextArea.flickable: TextArea {
                    id:textarea
                    readOnly: true
                    font.family: _fontFamily
                    font.pointSize: _fontPointSize
                    text:jsonString
                    color: "black"
                    wrapMode: TextArea.Wrap
                    background: Rectangle{
                        color: _colorRec
                        opacity: 0.2
                    }

                }

                ScrollBar.vertical: ScrollBar { }
            }
        }
    }
}
