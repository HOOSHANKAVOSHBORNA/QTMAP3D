import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    property string location
    property real lat
    property real lon
    property real  range
    property bool isselected: false
    id : recsaved
    height: 20
    width: 100
    color: "#88000000"
    radius: 5
    Connections{

    }

    Text {
        id :txt
        text:  location + " (" + lat + "," + lon + ")"
        font.pointSize: 10
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
    }
    MouseArea{
        anchors.fill: recsaved
        hoverEnabled: true
        onClicked: {
            Location.goPosition(lat,lon,range)
        }

        onEntered : {
            txt.font.bold =true
            recsaved.color = "#88006eff"
        }
        onExited: {

                txt.font.bold =false
                recsaved.color = "#88000000"

        }
    }
}
