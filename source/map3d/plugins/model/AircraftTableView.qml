
import QtQuick 2.13
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: rootItem
    anchors.fill: parent

    property AircraftTableModel model
    ScrollView {
        anchors.fill: parent
        TableView {
            model: rootItem.model

            delegate: Item {
                implicitWidth:   rct.implicitWidth + 4
                implicitHeight:  rct.implicitHeight + 4
                Rectangle {
                    id: rct
                    anchors.centerIn: parent
                    color: d_bkcolor
                    implicitWidth: 150
                    implicitHeight:  txt.implicitHeight + 10
                    Text {
                        id: txt
                        anchors.centerIn: parent
                        text: display
                        color: d_txtcolor
                    }
                }
            }

        }
    }
}
