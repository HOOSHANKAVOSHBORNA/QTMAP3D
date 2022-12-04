import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.13
Item {

    readonly property int _padding: 3


    property real longtitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0
    property real head : 0.0
    property real speed : 0.0
    property real time : 0.0

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
        }
    }

    GridLayout {
        id: gridLayout
        rows: 2
        columns: 2
        columnSpacing: 2
        rowSpacing: 3
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: _widgetmargin
        }

        Label {
            id: label1
            text: "<font color='black'>Longtitude: </font><font color='gray'>"+longtitude+"</font>"
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding : _padding
            //font.Family :_fontFamily

            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }

        Label {
            id: label
            text: "<font color='black'>Latitude: </font><font color='gray'>"+latitude+"</font>"
            padding : _padding
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }

        Label {
            id: label2
            text: "<font color='black'>Altitude: </font><font color='gray'>"+altitude+"</font>"
            padding : _padding
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }

        Label {
            id: label3
            text: "<font color='black'>Speed: </font><font color='gray'>"+speed+"</font>"
            padding : _padding
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }

        Label {
            id: label4
            text: "<font color='black'>Heading: </font><font color='gray'>"+head+"</font>"
            padding : _padding
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }
        Label {
            id: label5
            text: "<font color='black'>Time: </font><font color='gray'>"+time+"</font>"
            padding : _padding
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle{
                color: _colorRec
                opacity: 0.2
            }
        }



    }



}

