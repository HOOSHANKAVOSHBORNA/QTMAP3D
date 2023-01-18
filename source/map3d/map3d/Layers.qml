
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

Item {
    id: rootItem

    implicitWidth: 300
    property CLayersModel layersModel

    ColumnLayout {
        anchors.fill: parent

        Repeater {
            model: rootItem.layersModel


            ColumnLayout {

                Repeater {

                }
            }
        }
    }
}
