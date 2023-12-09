import QtQuick
import QtQuick.Layouts



Item {
    id:root
    anchors.fill: parent

    property var combatModel


    Rectangle{
        anchors.fill: parent
        radius: 10
        opacity: 0.7
        color: "blue"

        RowLayout{
            id:mainObject
            anchors.right: parent.right
            Image {
                id: mainObjectIcon
                source: combatModel.iconUrl
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log(combatModel.rowCount())
                    }
                }
            }
            Text {
                id: mainObjectTitle
                text: combatModel.title
            }

        }
        Rectangle{
            anchors.right: mainObject.left
            width: parent.width - mainObject.width
            height: parent.height *0.8
            anchors.verticalCenter: parent.verticalCenter

            GridView{

                id:se
                clip: true
                model: root.combatModel
                anchors.fill: parent
                delegate: Rectangle {
                    ColumnLayout{
                        anchors.fill: parent
                        Layout.alignment: Qt.AlignHCenter
                        // anchors.right: parent.right
                        Image {
                            source: objectIcon
                            sourceSize: ("40x40")
                        }
                        Text {
                            text: objectID
                            font.pixelSize: 12
                            color: stateColor
                        }
                    }
                }
            }
        }
    }
}



