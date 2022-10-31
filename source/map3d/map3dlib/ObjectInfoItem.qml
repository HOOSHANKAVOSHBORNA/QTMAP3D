import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13

Item {

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                clip: true
                contentWidth:  300
                contentHeight: 200

                Rectangle {
                    width:  300
                    height: 200
                    color: "red"

                    Rectangle {
                        anchors.centerIn: parent
                        width           : parent.width  / 2
                        height          : parent.height / 2
                        color           : "blue"
                    }
                }
            }
        }

        Item {
            Layout.fillWidth : true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                clip: true
                contentWidth:  300
                contentHeight: 200

                Rectangle {
                    width:  300
                    height: 200
                    color: "red"

                    Rectangle {
                        anchors.centerIn: parent
                        width           : parent.width  / 2
                        height          : parent.height / 2
                        color           : "blue"
                    }
                }
            }

        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                clip: true
                contentWidth:  300
                contentHeight: 200

                Rectangle {
                    width:  300
                    height: 200
                    color: "red"

                    Rectangle {
                        anchors.centerIn: parent
                        width           : parent.width  / 2
                        height          : parent.height / 2
                        color           : "blue"
                    }
                }
            }
        }
    }
}
