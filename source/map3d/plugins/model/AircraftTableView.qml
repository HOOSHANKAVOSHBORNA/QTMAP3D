
import QtQuick 2.13
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: rootItem
    anchors.fill: parent

    property AircraftTableModel model: AircraftTableModel {

    }

    TableView {
        anchors.fill: parent
        model: rootItem.model

        delegate: Rectangle {
            color: "red"
            implicitWidth:200
            implicitHeight:  30
            Text {
                anchors.centerIn: parent
                text: display
                color: "white"
            }
        }

    }
}
