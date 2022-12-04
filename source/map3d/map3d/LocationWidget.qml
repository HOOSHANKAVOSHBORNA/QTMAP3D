import QtQuick 2.0
import QtQuick.Controls 2.13

Item {
    id: element
    height: 40
    readonly property int _margin: 5
    property real longitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0
    Label {
        id: location
        text: "Lon Lat : ["+Number(longitude).toLocaleString(Qt.locale(), 'f', 3)+" , "+Number(latitude).toLocaleString(Qt.locale(), 'f', 3)+ " , "+Number(altitude).toLocaleString(Qt.locale(), 'f', 3)+"]"
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 3
        verticalAlignment: Text.AlignVCenter
        color: "#FFFFFF"
        padding: 3
        font.family: _fontFamily
        font.pointSize: location.width < 100 ? _fontPointSize - 5 : _fontPointSize
        clip: true


    }



}
