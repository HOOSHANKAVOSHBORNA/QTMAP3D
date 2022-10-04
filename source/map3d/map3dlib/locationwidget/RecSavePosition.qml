import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    property string location
    property real lat
    property real lon
    property bool isselected: false
    id : recsaved
    height: 20
    width: 100
    color: "#88000000"
    radius: 5

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
            for (var i in object){
                if (object[i].location === location){
                    if (!isselected){
                        recsaved.color = "#006eff"
                        isselected = true
                        Location.onCurrentClicked(location)
                    }else{
                        recsaved.color = "#88000000"
                        isselected = false
                    }
                }
                else {
                    object[i].color = "#88000000"
                    object[i].isselected =false
                }
            }
        }

        onEntered : {
            txt.font.bold =true
            recsaved.color = "#88006eff"
        }
        onExited: {
            if (!isselected){
                txt.font.bold =false
                recsaved.color = "#88000000"
            }
        }
    }
}
