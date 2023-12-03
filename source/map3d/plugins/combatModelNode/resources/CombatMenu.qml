import QtQuick
import QtQuick.Layouts

Item {
    width: parent.width/2
    height: 60
    Rectangle{
        anchors.fill: parent
        radius: 10
        opacity: 0.7
        color: blue

        RowLayout{
            id:mainObject
            anchors.right: parent.right
            Image {
                id: mainObjectIcon
                source: "file"
            }
            Text {
                id: mainObjectTitle
                text: qsTr("text")
            }
        }
        Rectangle{
            anchors.right: mainObject.left
            width: parent.width - mainObject.width
            height: parent.height *0.8
            anchors.verticalCenter: parent.verticalCenter

            ListView{
                clip: true
                anchors.centerIn: parent
                delegate: Item {
                    RowLayout{
                        anchors.right: parent.right
                        Image {
                            source: "file"
                        }
                        Text {
                            text: qsTr("text")
                        }
                    }
                }
            }
        }
    }
}
