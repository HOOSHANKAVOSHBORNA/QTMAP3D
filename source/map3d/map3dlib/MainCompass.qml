import QtQuick 2.13
import QtQuick.Window 2.13

Item {
    id:parent
    visible: true

       // Set the flag directly from QML
    Compass{
       anchors.centerIn: parent
    }

}
