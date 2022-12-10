import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
Item {

    property real longitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0
    Rectangle {
        id: statusbar
        anchors.fill: parent
        color: "#404040"
        RowLayout {
            anchors.leftMargin: 4
            anchors.left: parent.left
            Label {
                Layout.minimumWidth: hiddenn.implicitWidth
                id: lon
                color: "white"
                text: "Longitude: "+Number(longitude).toLocaleString(Qt.locale(), 'f', 3)
            }
            Label {
                id: lat
                Layout.minimumWidth: hiddenn.implicitWidth
                color: "white"
                text: "Latitude: "+Number(latitude).toLocaleString(Qt.locale(), 'f', 3)
            }
            Label {
                id: alt
                Layout.minimumWidth: hiddenn.implicitWidth
                color: "white"
                text: "Altitude: "+Number(altitude).toLocaleString(Qt.locale(), 'f', 3)
            }

        }
    }
    Label {
        id: hiddenn
        text: "Longitude: -254.000"
        visible: false
    }
}
