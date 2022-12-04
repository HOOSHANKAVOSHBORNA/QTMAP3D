import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    id:rootItem
    property string txt

    Rectangle {
        anchors.fill: parent
        color: "orangered"
    }

    Label {
        anchors.centerIn: parent
        text: rootItem.txt
        font.pointSize: 25
    }

}
