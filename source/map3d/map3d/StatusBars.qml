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
            Label {
                id: lon
                color: "white"
                text: "Longitude: "+Number(longitude).toLocaleString(Qt.locale(), 'f', 3)
            }
            Label {
                id: lat
                color: "white"
                text: "Latitude: "+Number(latitude).toLocaleString(Qt.locale(), 'f', 3)
            }
            Label {
                id: alt
                color: "white"
                text: "Altitude: "+Number(altitude).toLocaleString(Qt.locale(), 'f', 3)
            }
        }
    }
}
