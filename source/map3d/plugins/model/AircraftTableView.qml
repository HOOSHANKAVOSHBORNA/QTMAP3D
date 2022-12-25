
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

Item {
    id: rootItem
    anchors.fill: parent

    signal filterTextChanged(string txt)

    Timer {
        id: signalTimer
        running: false
        repeat: false
        interval: 500
        onTriggered: function() {
            rootItem.filterTextChanged(filterInput.text);
        }
    }

    property AircraftTableModel model
    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10

        Rectangle {
            Layout.alignment: Qt.AlignCenter
            Layout.minimumWidth: 320
            Layout.preferredWidth: 320
            Layout.minimumHeight: 40
            Layout.preferredHeight: 40
            color: "transparent"
            border.color: "white"
            radius: 5


            TextInput {
                id: filterInput
                anchors.centerIn: parent
                width: 300
                height: 40
                color: "white"
                clip: true
                onTextChanged: function() {
                    if (signalTimer.running === true) {
                        signalTimer.restart();
                    } else {
                        signalTimer.start();
                    }
                }
                verticalAlignment: Qt.AlignVCenter

            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
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
}
