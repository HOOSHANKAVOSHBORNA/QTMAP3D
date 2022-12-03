import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Button {
        anchors.centerIn: parent
        text: "OOOPS"

        onClicked: function() {
            console.log("OOPS clicked")
        }

    }
}
